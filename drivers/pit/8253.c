#include "drivers/pit/8253.h"
#include "drivers/lapic/lapic.h"
#include "asm.h"  
#include "idt.h" 
#include "timer.h" 
//#include "virtual_memory/vm.h"
//#include "drivers/pic/8259A.h" 

//#define K_STACK 0x1FFFFB

int static status = 0;

int counter = 0;

void init_pit()
{	
	static struct t_i_desc i_desc;
	out(BINARIY_COUNT_MODE,CONTROL_WORD);
	out(LOW_QUANTUM,COUNTER_0);
	out(HI_QUANTUM,COUNTER_0);
	
	i_desc.baseLow=(((u64)(&int_handler_pit)) & 0xFFFF);
	i_desc.selector=0x8;
	i_desc.flags=0x08e00;
	i_desc.baseHi=(((u64)(&int_handler_pit)) >> 0x010);
	i_desc.baseExt=(((u64)(&int_handler_pit)) >> (u64)0x020);
	i_desc.pad=0;	
	status = 1;
	set_idt_entry(0x22,&i_desc);
}

void free_pit()
{
	out(LOW_QUANTUM,COUNTER_0);
	out(HI_QUANTUM,COUNTER_0);
	status = 0;	
}

void int_handler_pit()
{
	t_llist_node* sentinel_node = NULL;
	t_llist_node* node = NULL;
	t_timer* timer = NULL;
	struct t_processor_reg processor_reg;
	struct t_processor_reg _processor_reg;

	SAVE_PROCESSOR_REG(processor_reg)
	//EOI_TO_LAPIC
	//SWITCH_DS_TO_KERNEL_MODE
	_processor_reg=processor_reg;
	counter++;
	if (status == 1)
	{
		sentinel_node = ll_sentinel(system.timer_list);
		node = ll_first(system.timer_list);
		while(node != sentinel_node)
		{
			timer = node->val;
			timer->val --;
			if (timer->val <= 0 )
			{
				break;
			}
			node = ll_next(node);
		}
	}

	EOI_TO_LAPIC
	RESTORE_PROCESSOR_REG(_processor_reg)                                                                                   	
	RET_FROM_INT_HANDLER   
}
