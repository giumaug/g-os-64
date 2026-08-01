#include "timer.h"
#include "asm.h"  
#include "idt.h" 
#include "virtual_memory/vm.h"
#include "drivers/lapic/lapic.h"
#include "drivers/pit/8253.h" 

extern u64 *AP_GDT;
extern u64 *AP_TSS;
extern u64 *AP_GDT_DESC;
extern u32 ap_trampoline_start;
extern u32 ap_trampoline_end;
extern u64 tmp_phy_kernel_stack[NUM_CPU - 1];

static void init_timer();
static void int_handler_lapic();
static u32 read_reg(u32 reg_offset);
static void write_reg(u32 reg_offset, u32 val);
static void set_timer_divisor(int divisor);

static u8 lapic_inizialized = 0;

void init_lapic()
{
	u32 val;
	u32 lapic_id;
	
    //map_vm_mem_static(LAPIC_BASE, PHY_LAPIC_BASE, PAGE_SIZE);
    map_vm_mem(system.master_page_pml4, LAPIC_BASE, PHY_LAPIC_BASE, PAGE_SIZE, 3);
	init_pit();
	// Clear Task Priority register; this enables all LAPIC interrupts
	val = read_reg(LAPIC_TPR);
	val &= ~0xff;
	write_reg(LAPIC_TPR, val);
	// Enable the LAPIC
	val = read_reg(LAPIC_SVR);
    val |= LAPIC_SVR_APIC_EN;
	write_reg(LAPIC_SVR, val);
	lapic_id = read_reg(LAPIC_ID);
	STI
	init_timer();
	CLI
	free_pit();
	lapic_inizialized = 1;
}

static void init_timer()
{
	static struct t_i_desc i_desc;
	t_timer* timer = NULL;
	u32 lapic_freq;
	u32 max_timer_count = 0xffffffff;
	u32 count;
	u32 divisor;
	
	timer = timer_init(0, NULL, NULL, NULL);
	for (divisor = 2; divisor <= 128; divisor *= 2)
	{
		set_timer_divisor(divisor);
		write_reg(LAPIC_TMR, LAPIC_TMR_ONESHOT | LAPIC_TMR_M);
		write_reg(LAPIC_ICR, max_timer_count);
		timer_set(timer, ONE_SEC_DELAY);
		while (timer->val > 0 );
		lapic_freq = max_timer_count - read_reg(LAPIC_CCR);
		if (lapic_freq != max_timer_count) 
		{
			break;
		}
	}
	if (divisor > 128)
	{
		panic();
	}

	i_desc.baseLow=(((u64)(&int_handler_lapic)) & 0xFFFF);
	i_desc.selector=0x8;
	i_desc.flags=0x08e00;
	i_desc.baseHi=(((u64)(&int_handler_lapic)) >> 0x010);
	i_desc.baseExt=(((u64)(&int_handler_lapic)) >> (u64)0x020);
	i_desc.pad=0;		
	set_idt_entry(0x38, &i_desc);
	
	count = lapic_freq / TICK_FRQ; //lapic_freq / count = TICK_FRQ
	write_reg(LAPIC_TMR, LAPIC_TMR_PERIODIC | LAPIC_TMR_VECTOR);
	write_reg(LAPIC_ICR, count);
}

static void set_timer_divisor(int divisor)
{
	unsigned int dv;
	switch (divisor) 
	{
     	case 1:
     		dv = LAPIC_LVT_DV_1;
			break;
       	case 2:
       		dv = LAPIC_LVT_DV_2;
			break;
       	case 4:
      		dv = LAPIC_LVT_DV_4;
			break;
       	case 8:
       		dv = LAPIC_LVT_DV_8;
			break;
      	case 16:
     		dv = LAPIC_LVT_DV_16;
			break;
      	case 32:
     		dv = LAPIC_LVT_DV_32;
			break;
      	case 64:
     		dv = LAPIC_LVT_DV_64;
			break;
    	case 128:
     		dv = LAPIC_LVT_DV_128;
			break;
   		default:
      		panic();
			break;
	}
	write_reg(LAPIC_DCR, dv);
}

static u32 read_reg(u32 reg_offset)
{
	unsigned char* address = NULL;
	u32 val;

	address = LAPIC_BASE + reg_offset;
	val = *((volatile u32*)(address));
	return val;
}

static void write_reg(u32 reg_offset, u32 val)
{
	unsigned char* address = NULL;

	address = LAPIC_BASE + reg_offset;
	(*((volatile u32*)(address))) = (val);
}

void int_handler_lapic()
{
	int is_schedule = 0;
	struct t_process_context* process_context = NULL;
	struct t_process_context* sleeping_process = NULL;
	struct t_processor_reg processor_reg;
	t_llist_node* next = NULL;
	t_llist_node* sentinel = NULL;
	t_llist_node* old_node = NULL;
	struct t_process_context* next_process = NULL;
	unsigned int queue_index;
	unsigned int priority;
	t_llist_node* sentinel_node = NULL;
	t_llist_node* node = NULL;
	t_llist_node* first_node = NULL;
	t_timer* timer = NULL;
	int cpuId;
	
	SAVE_PROCESSOR_REG(processor_reg)
	EOI_TO_LAPIC
	//SWITCH_DS_TO_KERNEL_MODE
	
	cpuId = get_current_process_context();
	if (cpuId == CPU_0)
	{
	  system.time += QUANTUM_DURATION;
	}
	if (system.int_path_count[cpuId] > 0)
	{
		goto EXIT_HANDLER;
	}
	sleeping_process = system.active_console_desc->sleeping_process[cpuId];
	sentinel = ll_sentinel(system.process_info->sleep_wait_queue[cpuId]);
	next = ll_first(system.process_info->sleep_wait_queue[cpuId]);
	next_process = next->val;
	//THIS STUFF MUST BE MOVED INSIDE ASSIGNED SLEEP MANAGER LIKE IO
	while(next != sentinel)
	{
		next_process->assigned_sleep_time -= QUANTUM_DURATION;
		next_process->sleep_time += QUANTUM_DURATION;
		
		if (next_process->sleep_time > 1000) 	
		{
			next_process->sleep_time = 1000;
		}
		else if (next_process->sleep_time < 0)
		{
			next_process->sleep_time = 0;
		}
		if (next_process->assigned_sleep_time == 0)
		{		
			adjust_sched_queue(next->val);			
			next_process->assigned_sleep_time = 0;
			next_process->proc_status = RUNNING;
			queue_index = next_process->curr_sched_queue_index;
			ll_append(system.scheduler_desc[cpuId]->scheduler_queue[queue_index], next_process);
			old_node = next;
			next=ll_next(next);
			ll_delete_node(old_node);
			is_schedule = 1;
		}
		else 
		{
			next=ll_next(next);
		}
		next_process = next->val;
	}

	if (sleeping_process != NULL && !system.active_console_desc->is_empty)
	{
		_awake(sleeping_process);
		system.active_console_desc->sleeping_process[cpuId] = NULL;
	}
	else
	{	
		process_context = system.process_info->current_process[get_current_process_context()]->val;
		process_context->sleep_time -= QUANTUM_DURATION;
		if (process_context->sleep_time > 1000) 	
		{
			process_context->sleep_time = 1000;
		}
		else if (process_context->sleep_time < 0)
		{
			process_context->sleep_time = 0;
		}

		if (process_context->proc_status == EXITING)
		{
			is_schedule = 2;
		}
		else 
		{
			process_context->tick--;
			if (process_context->tick == 0) 
			{
				process_context->tick = TICK;
				is_schedule = 1;	
			}
		}
	}

	//MANAGE TIMERS
EXIT_HANDLER:;
	sentinel_node = ll_sentinel(system.timer_list);
	node = ll_first(system.timer_list);
	first_node = node;
	do
	{
		timer = node->val;
		timer->val --;
		if (timer->val <= 0 )
		{
			(*timer->handler)(timer->handler_arg);
			break;
		}
		node = ll_next(node);
	}
	while(node != ll_first(system.timer_list));
//---	EXIT_INT_HANDLER(is_schedule,processor_reg,NULL);
    exit_int_handler(processor_reg, is_schedule, NULL);                                              
}

void ap_init()
{
	int i;
	u8* gdt_mem = &AP_GDT;
	u8* gdt_tss = &AP_TSS;
	u64 cpu_gdt[NUM_CPU - 1];
	u8* cpu_gtd_desc = &AP_GDT_DESC;
	
	for (i = 0; i < NUM_CPU - 1; i++)
	{
		cpu_gdt[i]  = &gdt_mem[(56 * i) + 0];
		//null descriptor
		gdt_mem[(56 * i) + 0] = 0x00;										
	    gdt_mem[(56 * i) + 1] = 0x00;
		gdt_mem[(56 * i) + 2] = 0x00;
		gdt_mem[(56 * i) + 3] = 0x00;
		gdt_mem[(56 * i) + 4] = 0x00;
		gdt_mem[(56 * i) + 5] = 0x00;
		gdt_mem[(56 * i) + 6] = 0x00;
		gdt_mem[(56 * i) + 7] = 0x00;

        // gdt kernel code segment
		gdt_mem[(56 * i) + 8] = 0x00;										
		gdt_mem[(56 * i) + 9] = 0x00;
		gdt_mem[(56 * i) + 10] = 0x00;
		gdt_mem[(56 * i) + 11] = 0x00;
		gdt_mem[(56 * i) + 12] = 0x00;
		gdt_mem[(56 * i) + 13] = 0x9A;
		gdt_mem[(56 * i) + 14] = 0x20;
		gdt_mem[(56 * i) + 15] = 0x00;

		//gdt kernel data segment
		gdt_mem[(56 * i) + 16] = 0x00;										
		gdt_mem[(56 * i) + 17] = 0x00;
		gdt_mem[(56 * i) + 18] = 0x00;
		gdt_mem[(56 * i) + 19] = 0x00;
		gdt_mem[(56 * i) + 20] = 0x00;
		gdt_mem[(56 * i) + 21] = 0x92;
		gdt_mem[(56 * i) + 22] = 0x00;
		gdt_mem[(56 * i) + 23] = 0x00;

		//# gdt user code segment
		gdt_mem[(56 * i) + 24] = 0x00;										
		gdt_mem[(56 * i) + 25] = 0x00;
		gdt_mem[(56 * i) + 26] = 0x00;
		gdt_mem[(56 * i) + 27] = 0x00;
		gdt_mem[(56 * i) + 28] = 0x00;
		gdt_mem[(56 * i) + 29] = 0xFA;
		gdt_mem[(56 * i) + 30] = 0x20;
		gdt_mem[(56 * i) + 31] = 0x00;

		//gdt user data segment
		gdt_mem[(56 * i) + 32] = 0x00;										
		gdt_mem[(56 * i) + 33] = 0x00;
		gdt_mem[(56 * i) + 34] = 0x00;
		gdt_mem[(56 * i) + 35] = 0x00;
		gdt_mem[(56 * i) + 36] = 0x00;
		gdt_mem[(56 * i) + 37] = 0xF2;
		gdt_mem[(56 * i) + 38] = 0x00;
		gdt_mem[(56 * i) + 39] = 0x00;

		//gdt tss segment
		gdt_mem[(56 * i) + 40] = 0x68;							
		gdt_mem[(56 * i) + 41] = 0x00;
		gdt_mem[(56 * i) + 42] = ((u64)(gdt_tss + (104 * i))) & 0xFFULL;
		gdt_mem[(56 * i) + 43] = (((u64)(gdt_tss + (104 * i))) & 0xFF00ULL) >> 8;
		gdt_mem[(56 * i) + 44] = (((u64)(gdt_tss + (104 * i))) & 0xFF0000ULL) >> 16;
		gdt_mem[(56 * i) + 45] = 0x89;
		gdt_mem[(56 * i) + 46] = 0x00;
		gdt_mem[(56 * i) + 47] = (((u64) (gdt_tss + (104 * i))) & 0xFF000000ULL) >> 24;
		gdt_mem[(56 * i) + 48] = (((u64)(gdt_tss + (104 * i))) & 0xFF00000000ULL) >> 32;
		gdt_mem[(56 * i) + 49] = (((u64)(gdt_tss + (104 * i))) & 0xFF0000000000ULL) >> 40;
		gdt_mem[(56 * i) + 50] = (((u64)(gdt_tss + (104 * i))) & 0xFF000000000000ULL) >> 48;
		gdt_mem[(56 * i) + 51] = (((u64)(gdt_tss + (104 * i))) & 0xFF00000000000000ULL) >> 56;
		gdt_mem[(56 * i) + 52] = 0x00;
		gdt_mem[(56 * i) + 53] = 0x00;
		gdt_mem[(56 * i) + 54] = 0x00;
		gdt_mem[(56 * i) + 55] = 0x00;
		
		*((u16*)(cpu_gtd_desc + (16 * i))) = 55;
		*((u64*)(cpu_gtd_desc + (16 * i) + 2)) = gdt_mem + (56 * i);
	}
	//stack setup
	//kernel_stack_addr = buddy_alloc_page(system.buddy_desc,KERNEL_STACK_SIZE);
	
	*((volatile u32*)(LAPIC_BASE + 0x280)) = 0;
	*((volatile u32*)(LAPIC_BASE + 0x310)) = (*((volatile u32*)(LAPIC_BASE + 0x310))) & 0xFFFFFF;
	*((volatile u32*)(LAPIC_BASE + 0x300)) = 0xC0500;
	do { __asm__ __volatile__ ("pause" : : : "memory"); }while(*((volatile u32*)(LAPIC_BASE + 0x300)) & (1 << 12));
	
	*((volatile u32*)(LAPIC_BASE + 0x280)) = 0;
	*((volatile u32*)(LAPIC_BASE + 0x310)) = (*((volatile u32*)(LAPIC_BASE + 0x310))) & 0xFFFFFF;
	*((volatile u32*)(LAPIC_BASE + 0x300)) = 0xC0608;
	do { __asm__ __volatile__ ("pause" : : : "memory"); }while(*((volatile u32*)(LAPIC_BASE + 0x300)) & (1 << 12));
}

u8 get_current_process_context()
{
	u8 id;
	u8 mapId = 0;
	
	if (lapic_inizialized == 0)
	{
		return 0;
	}
	id = read_reg(LAPIC_ID);
	switch (id) 
	{
		case CPU_0:
		{
			mapId = 0;
			break;
		}
		case CPU_1:
		{
			mapId = 1;
			break;
		}
		case CPU_2:
		{
			mapId = 2;
			break;
		}
		case CPU_3:
		{
			mapId = 3;
			break;
		}
		case CPU_4:
		{
			mapId = 4;
			break;
		}
		case CPU_5:
		{
			mapId = 5;
			break;
		}
		case CPU_6:
		{
			mapId = 6;
			break;
		}
		case CPU_7:
		{
			mapId = 7;
			break;
		}
		case CPU_8:
		{
			mapId = 8;
			break;
		}
		case CPU_9:
		{
			mapId = 9;
			break;
		}
		case CPU_10:
		{
			mapId = 10;
			break;
		}
		case CPU_11:
		{
			mapId = 11;
			break;
		}
		case CPU_12:
		{
			mapId = 12;
			break;
		}
		case CPU_13:
		{
			mapId = 13;
			break;
		}
		case CPU_14:
		{
			mapId = 14;
			break;
		}
		case CPU_15:
		{
			mapId = 15;
			break;
		}
		case CPU_16:
		{
			mapId = 16;
			break;
		}
		case CPU_17:
		{
			mapId = 17;
			break;
		}
		case CPU_18:
		{
			mapId = 18;
			break;
		}
		case CPU_19:
		{
			mapId = 19;
			break;
		}
		case CPU_20:
		{
			mapId = 20;
			break;
		}
		case CPU_21:
		{
			mapId = 21;
			break;
		}
		case CPU_22:
		{
			mapId = 22;
			break;
		}
		case CPU_23:
		{
			mapId = 23;
			break;
		}
		case CPU_24:
		{
			mapId = 24;
			break;
		}
		case CPU_25:
		{
			mapId = 25;
			break;
		}
		case CPU_26:
		{
			mapId = 26;
			break;
		}
		case CPU_27:
		{
			mapId = 27;
			break;
		}
		case CPU_28:
		{
			mapId = 28;
			break;
		}
		case CPU_29:
		{
			mapId = 29;
			break;
		}
		case CPU_30:
		{
			mapId = 30;
			break;
		}
		case CPU_31:
		{
			mapId = 31;
			break;
		}
	}
	return mapId;
}

void relocate_init_code()
{
	kmemcpy(AP_TRAMPOLINE_DST_ADDR, AP_TRAMPOLINE_SRC_ADDR, AP_TRAMPOLINE_SIZE);
}

void ap_post_init(int cpuId)
{
  static unsigned int params[1];
  static struct t_process_context* process_context = NULL;
  static u64 kernel_stack;
  static int _cpuId;
    
  _cpuId = cpuId;
  process_context = kmalloc(sizeof(struct t_process_context));
  u64 xxx = buddy_alloc_page(system.buddy_desc,KERNEL_STACK_SIZE);
  process_context->phy_kernel_stack = FROM_VIRT_TO_PHY(buddy_alloc_page(system.buddy_desc,KERNEL_STACK_SIZE));
  init_vm_process(process_context);
  
  kernel_stack = KERNEL_STACK - 100;
  asm volatile ("mov %0,%%rbp;"::"r"(kernel_stack));
  asm volatile ("mov %0,%%rsp;"::"r"(kernel_stack));
  
  buddy_free_page(system.buddy_desc, tmp_phy_kernel_stack[_cpuId - 1]);
  system.process_info->current_process[_cpuId] = ll_prepend(system.scheduler_desc[_cpuId]->scheduler_queue[9],process_context);
  
  params[0]=0;       
  SYSCALL(13L,params);
}
	




