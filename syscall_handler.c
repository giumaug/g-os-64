#include "asm.h"
#include "idt.h"
#include "scheduler/scheduler.h"
#include "virtual_memory/vm.h"
#include "syscall_handler.h"
#include "drivers/ioapic/ioapic.h"
#include "debug.h"
#include "ext2/ext2.h"
#include "system.h"

#define K_STACK 0x1FFFFB

#define SELECT_FS(ext2) if (system.device_desc->num == 0)        \
			{                                                    \
				ext2 = system.root_fs;                           \
			}                                                    \
			else                                                 \
			{                                                    \
				ext2 = system.scnd_fs;                           \
			}                                                    \

void syscall_post_handler(t_spinlock_desc* lock)
{
	int cpuIndex;
	
	cpuIndex = get_current_process_context(); 
	if (lock != NULL)
	{
		SPINLOCK_UNLOCK(*lock,cpuIndex);
		ENABLE_PREEMPTION(cpuIndex)
	}	
}

void syscall_handler()
{
	static int free_vm_proc;        
	static int syscall_num;
	unsigned int mem_size;
	struct t_process_context* current_process_context;
	struct t_processor_reg processor_reg;
	u64* params = NULL;
	char data;
	unsigned int on_exit_action;
	u8 flush_network;
	t_ext2* ext2 = NULL;
	t_post_handler post_handler;
	
 	SAVE_PROCESSOR_REG(processor_reg)
	//call can come from kernel mode (sleep)
	SWITCH_SS_TO_KERNEL_MODE
	syscall_num=processor_reg.rax;
	on_exit_action=0;
	current_process_context=system.process_info->current_process[get_current_process_context()]->val;
	
	t_console_desc *console_desc=current_process_context->console_desc;
	syscall_num=processor_reg.rax;
	params=processor_reg.rcx;
	flush_network = 0;

	switch (syscall_num) 
	{
		case 1:
		params[0]=_fork(processor_reg);
		break;
	
		case 2:
		params[1]=_malloc(params[0]);
		break;	

		case 3:
		_free(params[0]);
		break;

		case 4:
		_write_char(console_desc,params[0]);
		break;	

		case 5:
		data=_read_char(console_desc);
		*((char*)params[0])=data;	
		if (data==NULL)
		{
			on_exit_action=1; 
		}
		break;
	
		case 6:
		_echo_char(console_desc,params[0]);
		break;	

		case 7:
		_enable_cursor(console_desc);
		break;	
	
		case 8:
		_disable_cursor(console_desc);
		break;
	
		case 9:
		_update_cursor(console_desc);
		break;	
	
		case 10:
		_delete_char(console_desc);
		break;
	
		case 11:
		_pause();	
		on_exit_action=1;
		break; 
	
		case 12:
		_awake(params[0]);
		break;

		case 13:
		_exit(params[0]);
		on_exit_action=2;
		break;
	
		case 14: 
		params[2]=_exec(params[0],params[1]);
		break; 
	
		case 15:
		_sleep_time(params[0]);	
		on_exit_action=1;
		break; 
	
		case 18:
		//--SELECT_FS(ext2)
		if (system.device_desc->num == 0)        
		{                                        
			ext2 = system.root_fs;          
		}                                        
		else                                     
		{                                        
			ext2 = system.scnd_fs;           
		}   
	 	params[2] = _open(ext2,(char*) params[0],params[1]);
		on_exit_action=1;
		break; 

		case 19:
		SELECT_FS(ext2) 
		params[1]=_close(ext2,params[0]);
		on_exit_action=1; 
		break;

		case 20:
		SELECT_FS(ext2)
	 	params[3]=_read(ext2,params[0],params[1],params[2],1);
		on_exit_action=1; 
		break;

		case 36:
		SELECT_FS(ext2)
		params[3] = _seek(ext2,params[0],params[1],params[2]);
		on_exit_action=1; 
		break;
	
		case 22:
		SELECT_FS(ext2)
		params[1]=_rm(ext2,(char*)params[0]);
		on_exit_action=1; 
		break;

		case 23:
		SELECT_FS(ext2)
		params[1]=_mkdir(ext2,params[0]);
		on_exit_action=1; 
		break;

		case 26:
		SELECT_FS(ext2)
		params[1]=_chdir(system.root_fs,(char*) params[0]); 
		on_exit_action=1;
		break; 	
	
		case 27:
		SELECT_FS(ext2)
		params[2]=_stat(system.root_fs,(char*) params[0],params[1]);
		break;

		case 28:
 		params[1]=_open_socket(params[0]);
		break; 
	
		case 29:
 		params[5]=_bind(params[0],params[1],params[2],params[3],params[4]);
		break;	

		case 30:
 		params[5]=_recvfrom(params[0],params[1],params[2],params[3],params[4]);
		break;
	
		case 31:
 		params[5]=_sendto(params[0],params[1],params[2],params[3],params[4]);
		break;
	
		case 32:
 		params[0]=_close_socket(params[0]);
		break;
	
		case 33:
 		params[1]=_listen(params[0]);
		break;
	
		case 34:
 		params[3]=_accept(params[0]);
		break;
	
		case 35:
 		params[3]=_connect(params[0],params[1],params[2]);
		break;

		case 37:
 		params[3] = _icmp_echo_request(params[0],params[1],params[2]);        
		break;

		case 38:
 		params[0] = _getpid();        
		break;

		case 39:
 		params[1] = _getpgid(params[0]);
		break;

		case 40:
 		params[2] = _setpgid(params[0],params[1]);      
		break;

		case 41:
 		params[0] = _tcgetpgrp();      
		break;

		case 42:
 		params[1] = _tcsetpgrp(params[0]);   
		break;
		
		case 101: 
		on_exit_action=1;
		//post_handler.exec = &syscall_post_handler;
		//post_handler.arg = params[0];
		break;
	
		case 106:
		params[1]=_listen(params[0]);
		break;

		case 107:
		_signal();
		break;

		case 109:
		SELECT_FS(ext2)
	 	params[4] = _read_write(ext2, params[0], params[1], params[2], params[3], 1);
		on_exit_action = 1;
		break;

		case 103:
		check_free_mem();
		break;

        case 104:
		panic();
		break;
		
		case 200:
		flush_inode_cache(system.root_fs);
		break;	

		default:                                                                                   
		panic();
	}
	if (syscall_num == 30 || syscall_num == 31 || syscall_num == 32 || syscall_num == 35)
	{
		system.flush_network = 1;
	}
	exit_int_handler(processor_reg, on_exit_action, params);
    //EXIT_INT_HANDLER(on_exit_action,processor_reg, post_handler.exec)
}
