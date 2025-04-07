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

	SAVE_PROCESSOR_REG
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
	RESTORE_PROCESSOR_REG                                                                                   	
	RET_FROM_INT_HANDLER
//-	EXIT_INT_HANDLER(0, processor_reg) 
                                                                    \
/*                                                                                                              
	static struct t_process_context _current_process_context;                                                  		
	static struct t_process_context _old_process_context;                                                      		
	static struct t_process_context _new_process_context;	                                                        
	static struct t_processor_reg _processor_reg;                                                                   
	static unsigned int _action2;                                                                                   
    static u8 stop = 0;   
    int action = 0;                                                                                          
                                                                                                                    
	CLI                                                                                                             
	if (system.int_path_count == 0 && system.force_scheduling == 0 && system.flush_network == 1)                    
	{                                                                                                               
		system.flush_network = 0;                                                                       	        
		dequeue_packet(system.network_desc);                                                            	        
		equeue_packet(system.network_desc);                                                             	        
		system.flush_network = 1;                                                                       	        
	}                                                                                                               
	_action2=action;                                                                                                
	_current_process_context = *(struct t_process_context*)system.process_info->current_process->val;                 
	_old_process_context = _current_process_context;                                                                  
	_processor_reg=processor_reg;                                                                                   
	                                                                                                                
	if (system.force_scheduling == 1 && action == 0 && system.int_path_count == 0)                                  
	{                                                                                                               
		_action2 = 1;                                                                                           	
		if (_current_process_context.proc_status == EXITING)                                                    	
		{                                                                                                       	
			_action2 = 2;                                                                                   	    
		}                                                                                                       	
	}                                                                                                               
                                                                                                                    
	if (_action2>0)                                                                                                 
	{	                                                                                                            
		system.force_scheduling = 0;                                                                            	
        stop = 0;                                                                                               	
		while(!stop)                                                                                            	
		{                                                                                                       	
			schedule(&_current_process_context, &_processor_reg);                                            	    
			_new_process_context = *(struct t_process_context*) system.process_info->current_process->val;  	    
			if (_new_process_context.sig_num == SIGINT)                                                     	    
			{                                                                                               	    
				free_vm_process(&_new_process_context);                                                      	    
				buddy_free_page(system.buddy_desc, FROM_PHY_TO_VIRT(_new_process_context.phy_kernel_stack));  	    
			}                                                                                               	    
			else                                                                                            	    
			{                                                                                               	    
				stop = 1;                                                                               	        
			}                                                                                               	    
		}                                                                                                       	
                                                                                                                    
		if (_new_process_context.pid != _old_process_context.pid)                                               	
		{                                                                                                       	
			_processor_reg=_new_process_context.processor_reg;                                      	            
		}                                                                                                       	
		SWITCH_PAGE_DIR(FROM_VIRT_TO_PHY((_new_process_context.page_pml4)))                       	
		DO_STACK_FRAME(_processor_reg.rsp - 8);                                                                   	
                                                                                                                    
		if (_action2 == 2)                                                                                        	
		{                                                                                                       	
			DO_STACK_FRAME(_processor_reg.rsp-8);                                                           	    
			free_vm_process(&_old_process_context);                                                         	    
			buddy_free_page(system.buddy_desc, FROM_PHY_TO_VIRT(_old_process_context.phy_kernel_stack));     	    
		}                                                                                                       	
		RESTORE_PROCESSOR_REG                                                                                   	
		EXIT_SYSCALL_HANDLER                                                                                    	
	}                                                                                                          		
	else                                                                                                       		
	{                                                                                                               
		RESTORE_PROCESSOR_REG                                                                                   	
		RET_FROM_INT_HANDLER                                                                                	
	}
*/      
}
