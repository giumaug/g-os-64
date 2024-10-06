#include "multiboot.h"
#include "system.h"
#include "asm.h"
#include "idt.h"
#include "scheduler/process.h"
#include "memory_manager/kmalloc.h"
#include "memory_manager/buddy.h"
#include "virtual_memory/vm.h"
#include "syscall_handler.h"
#include "network/network.h"
#include "drivers/kbc/8042.h"
#include "drivers/ahci/ahci.h"
#include "drivers/lapic/lapic.h"
#include "framebuffer/framebuffer.h"
#include "drivers/ioapic/ioapic.h"

void process_0();

//to fix when file system working!!
unsigned int seed=105491;
extern unsigned int PAGE_DIR;
t_system system;

void kmain(multiboot_info_t* mbd, unsigned int magic, unsigned int init_data_add)
{
	u32 device_num;
	static multiboot_info_t _mbd;
	_mbd = *mbd;
	static struct t_process_info process_info;
	static t_scheduler_desc scheduler_desc;
//	static t_buddy_desc buddy_desc;
	static unsigned int* init_data;
    init_data = init_data_add;
	static struct t_process_context* process_context = NULL;
	static struct t_i_desc i_desc;
	static t_console_desc console_desc;
	static t_ext2 ext2_d1;
	static t_ext2 ext2_d2;
	static u32 kernel_stack;
	static t_ahci_device_desc* device_desc_ahci = NULL;
	t_device_desc* device_desc = NULL;
	system.time = 0;
	system.flush_network = 0;
    system.read_block_count = 0;
	system.read_bitmap_count = 0;
	system.read_write_count = 0;
	system.run_time = 0;
	system.run_time_1 = 0;
	device_num = 0;
	
	u64* xxx;
	
	init_data = init_data_add;
 	CLI
	system.force_scheduling = 0;
	system.process_info = &process_info;
	//system.buddy_desc = &buddy_desc;
	system.scheduler_desc = &scheduler_desc;
	system.int_path_count = 0;
	system.scheduler_desc->scheduler_queue[0] = 0;
	system.process_info->current_process = NULL;
	
	system.master_page_pml4 = (void*)init_virtual_memory();
	SWITCH_PAGE_DIR(system.master_page_pml4)
	
	init_kmallocs();
	
	xxx = kmalloc(24);
	kfree(xxx);
	
	init_idt();
	system.buddy_desc = buddy_init();
	init_scheduler();
	
	system.timer_list = new_dllist();
	init_ioapic();
	init_lapic();
	
	init_kbc();
	init_fb(mbd);
	init_console(&console_desc, &draw_char_fb, &update_cursor_fb, 2, 0);
	
	device_desc = init_device(device_num, 2);
	device_desc_ahci = init_ahci(device_desc);
	system.device_desc = device_desc;
	init_ext2(&ext2_d1,system.device_desc);
	system.root_fs = &ext2_d1;
	
	system.active_console_desc = &console_desc;
	i_desc.baseLow = ((u16) &syscall_handler) & 0xFFFF;
	i_desc.selector = 0x8;
	i_desc.flags = 0x0EF00; 
	i_desc.baseHi = ((u16) &syscall_handler)>>0x10;
	i_desc.baseExt=((u32)( &syscall_handler)) >> 0x020;
    i_desc.pad=0;
	set_idt_entry(0x80,&i_desc);

	system.process_info->sleep_wait_queue = new_dllist();	
	system.process_info->next_pid = 1;
	system.process_info->pid_hash = hashtable_init(PID_HASH_SIZE);
	system.process_info->pgid_hash = hashtable_init(PGID_HASH_SIZE);
	
	process_context = kmalloc(sizeof(struct t_process_context));
	process_context->root_dir_inode_number = ROOT_INODE;
	process_context->current_dir_inode_number = ROOT_INODE;
	process_context->proc_status = RUNNING;
	process_context->assigned_sleep_time = 0;	
	process_context->sleep_time = 0;
	process_context->static_priority = 0;
	process_context->curr_sched_queue_index = 9;
	process_context->pid = 0;
	process_context->pgid = 0;
	process_context->sig_num = 0;
	process_context->sleep_wait_queue_ref = 0;
	hashtable_put(system.process_info->pgid_hash,0,new_dllist());
	
    process_context->tick = TICK;
	process_context->processor_reg.rsp = NULL;
	process_context->console_desc = &console_desc;
	system.process_info->current_process = ll_prepend(system.scheduler_desc->scheduler_queue[9],process_context);
	system.process_info->tss.ss = *init_data;
	system.process_info->tss.esp = *(init_data+1);
	system.process_info->pause_queue = new_dllist();
	process_context->phy_kernel_stack = FROM_VIRT_TO_PHY(buddy_alloc_page(system.buddy_desc,KERNEL_STACK_SIZE));
	process_context->process_type = KERNEL_THREAD;
	process_context->file_desc = dc_hashtable_init(PROCESS_INIT_FILE, NULL);
	process_context->socket_desc = hashtable_init(PROCESS_INIT_SOCKET);
	process_context->next_sd = 0;
	process_context->sig_num = 0;
	 
	process_context->page_pml4 = buddy_alloc_page(system.buddy_desc,0x1000);                      
	init_vm_process(process_context);
	*(system.process_info->tss.ss) = 0x18;
	*(system.process_info->tss.esp) = KERNEL_STACK;
	system.network_desc = network_init();
	//system.network_desc = NULL;
	//system.timer_list = new_dllist();                       		
	kernel_stack = KERNEL_STACK - 100;
	asm volatile ("movl %0,%%ebp;"::"r"(kernel_stack));
	asm volatile ("movl %0,%%esp;"::"r"(kernel_stack));
	
//	int i;
//	for (i = 0; i < 25000; i++)
//	{
//			(system.tcp_fin_status[i]).status = 0;
//			system.tcp_fin_status[i].conn = 0;
//	}
	
	STI
	process_0();	       	
}

/*
diplay_test(multiboot_info_t* mbd, void* page_dir)
{
	int i;	            
	long fb_virt_addr = 0xc5A7000; 
	long fb_phy_addr = mbd->framebuffer_addr;
	char* fbdata = NULL;
	
	int fb_width = mbd->framebuffer_width;
	int fb_height = mbd->framebuffer_height;
	int fb_bpp = mbd->framebuffer_bpp;
	int fb_bytes = fb_bpp / 8; 

	int fb_data_size = fb_width * fb_height * fb_bytes;
	map_vm_mem(page_dir, fb_virt_addr,fb_phy_addr, (fb_data_size + PAGE_SIZE), 7);
	SWITCH_PAGE_DIR(FROM_VIRT_TO_PHY(((unsigned int)page_dir)))
	
	fbdata = fb_virt_addr;
	for (i = 0; i < fb_data_size; i++)
	{
		fbdata[i] = 0;
	}

	int offset = (100 * fb_width + 100) * 4;
	int r = 255; 
	int g = 255; 
	int b = 255;

	for (i = 0; i < 100; i++)
	{
		fbdata[offset + 0 + i] = b;
		fbdata [offset + 1 + i] = g;
		fbdata[offset + 2 + i] = r;
		fbdata[offset + 3 + i] = 0; // May not be neeeded
	}
	while(1);
}
*/
