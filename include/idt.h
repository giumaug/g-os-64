#ifndef IDT_H                
#define IDT_H

#include "system.h"
#include "scheduler/process.h"

void post_context_switch(u8 cpu_id);
void exit_int_handler(struct t_processor_reg processor_reg, int on_exit_action, u64* params);

struct t_i_desc {
   u16 baseLow;    	 
   u16 selector;      	
   u16 flags;
   u16 baseHi;
   u32 baseExt;
   u32 pad;
};

struct t_idt_ptr {
	u16 idt_size __attribute__((__packed__));
	u64 idt_address __attribute__((__packed__));
};

typedef struct s_post_handler {
    void (*exec)(void*);
    void* arg;               
} t_post_handler;

void int_handler_generic();
void init_idt();
void set_idt_entry(int entry,struct t_i_desc* i_desc);
 
#endif



