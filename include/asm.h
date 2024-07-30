#ifndef ASM_H                
#define ASM_H

#include "system.h"

#define GET_FROM_STACK(n,var) asm("mov %%rbp,%0;":"=r"(var));var+=n+2;
#define STI asm ("sti");
#define CLI asm ("cli");
#define EXIT_SYSCALL_HANDLER asm("pop %rbp;iret;");

#define SAVE_PROCESSOR_REG asm("push %rbp");                           \
                asm("push %rdi");                                     \
                asm("push %rsi");                                     \
                asm("push %rdx");                                     \
                asm("push %rcx");                                     \
                asm("push %rbx");                                     \
                asm("movq %%rax, %0;":"=r"(processor_reg.rax));        \
                asm("pop %rbx");                                      \
                asm("movq %%rbx, %0;":"=r"(processor_reg.rbx));        \
                asm("pop %rcx");                                      \
                asm("movq %%rcx, %0;":"=r"(processor_reg.rcx));        \
                asm("pop %rdx");                                      \
                asm("movq %%rdx, %0;":"=r"(processor_reg.rdx));        \
                asm("pop %rsi");                                      \
                asm("movq %%rsi, %0;":"=r"(processor_reg.rsi));        \
                asm("pop %rdi");                                      \
                asm("movq %%rdi, %0;":"=r"(processor_reg.rdi));        \
                asm("pop %rbp");                                      \
                asm("movq %%rbp, %0;":"=r"(processor_reg.rsp));        \
                asm("pop %r8");                                       \
                asm("movq %%r8, %0;":"=r"(processor_reg.r8));          \
                asm("pop %r9");                                       \
                asm("movq %%r9, %0;":"=r"(processor_reg.r9));          \
                asm("pop %r10");                                      \
                asm("movq %%r10, %0;":"=r"(processor_reg.r10));        \
                asm("pop %r11");                                      \
                asm("movq %%r11, %0;":"=r"(processor_reg.r11));       \
                asm("pop %r12");                                      \
                asm("movq %%r12, %0;":"=r"(processor_reg.r12));       \
                asm("pop %r13");                                      \
                asm("movq %%r13, %0;":"=r"(processor_reg.r13));        \
                asm("pop %r14");                                      \
                asm("movq %%r14, %0;":"=r"(processor_reg.r14));        \
                asm("pop %r15");                                      \
                asm("movq %%r15, %0;":"=r"(processor_reg.r15));
                
// PUSH AND POP REGITERS ON STACK TO AVIOD INLINE ASM DIRTIES THEM
#define RESTORE_PROCESSOR_REG	asm ("mov %0,%%rsp;"::"r"(_processor_reg.rsp)); \
                asm ("movq %0,%%rax;"::"r"(_processor_reg.rax));  \
                asm ("push %rax;");                              \
                asm ("movq %0,%%rbx;"::"r"(_processor_reg.rbx));  \
                asm ("push %rbx;");                              \
                asm ("movq %0,%%rcx;"::"r"(_processor_reg.rcx));  \
                asm ("push %rcx;");                              \
                asm ("movq %0,%%rdx;"::"r"(_processor_reg.rdx));  \
                asm ("push %rdx;");                              \
                asm ("movq %0,%%rsi;"::"r"(_processor_reg.rsi));  \
                asm ("push %rsi;");                              \
                asm ("movq %0,%%rdi;"::"r"(_processor_reg.rdi));  \
                asm ("push %rdi;");                               \
                asm ("movq %0,%%r8;"::"r"(_processor_reg.r8));    \
                asm ("push %r8;");                               \
                asm ("movq %0,%%r9;"::"r"(_processor_reg.r9));    \
                asm ("push %r10;");                              \
                asm ("movq %0,%%r10;"::"r"(_processor_reg.r10));  \
                asm ("push %r11;");                              \
                asm ("movq %0,%%r11;"::"r"(_processor_reg.r11));  \
                asm ("push %r12;");                              \
                asm ("movq %0,%%r12;"::"r"(_processor_reg.r12));  \
                asm ("push %r13;");                              \
                asm ("movq %0,%%r13;"::"r"(_processor_reg.r13));  \
                asm ("push %r14;");                              \
                asm ("movq %0,%%r14;"::"r"(_processor_reg.r14));  \
                asm ("push %r15;");                              \
                asm ("movq %0,%%r15"::"r"(_processor_reg.r15));
                asm ("pop %r15;");                                \
                asm ("pop %r14;");                                \
                asm ("pop %r13;");                                \
                asm ("pop %r12;");                                \
                asm ("pop %r11;");                                \
                asm ("pop %r10;");                                \
                asm ("pop %r9;");                                 \
                asm ("pop %r8;");                                 \
				asm ("pop %rdi;");                                \
				asm ("pop %rsi;");                                \
				asm ("pop %rdx;");                                \
				asm ("pop %rcx;");                                \
				asm ("pop %rbx;");                                \
				asm ("pop %rax;");
                	                
#define RET_FROM_INT_HANDLER 	asm("movq %rbp,%rsp;popq %rbp;iret");
                    	
#define SWITCH_TO_USER_MODE(stack_address)                                   \
                    asm("mov %0,%%rax;push %%rax;"::"r"(stack_address));    \
                    asm("mov $0x206,%rax;                                   \
                         push %rax;                                         \
                         mov $0x13,%eax;      /*cs*/                        \
                         push %rax;                                         \
                         mov $0x100000,%eax;  /*eip*/                       \
                         push %rax;                                         \
                         iret;");

//IN ORDER TO PROTECT EAX I NEED BOTH TO PUSH AND POP IT ON STACK AND HITS ABOUT IT ASM INLNE
//ASM INLINE DOSN'T KNOW REGARDING FINAL POP
#define GET_FAULT_ADDRESS(fault_addr,fault_code) asm ("		   \
                            push %%rax;                        \
                            mov %%cr2,%%rax;                  \
                            mov  %%rax, %0;                   \
                            mov  8(%%rbp), %1;                \
                            pop %%rax;"                        \
                            :"=r"(fault_addr),"=r"(fault_code) \
                            ::"%rax"                           \
                            );

#define EOI_TO_MASTER_PIC asm("mov $0x20,%%al;                  \
                              out %%al,$0x20;"                 \
                              :                                \
                              :                                \
                              :"%eax");

#define EOI_TO_SLAVE_PIC asm("mov $0x20,%%al;                   \
                             out %%al,$0xa0;"                  \
                             :                                 \
                             :                                 \
                             :"%eax");

#define SAVE_IF_STATUS unsigned int if_status;                  \
                      asm("push %rax;                          \
                           pushfq;                             \
                           mov (%rsp),%rax;                    \
                           and $0x200,%rax;                    \
	                      ");                                  \
                      asm("mov %%eax,%0;":"=r"(if_status)::"%eax");	\
                      asm("popfq;                                   \
                           pop %rax;                                \
                          ");

#define RESTORE_IF_STATUS asm("push %rax;                            \
                              pushfq;                               \
                             ");                                    \
                         asm("mov %0,%%eax;"::"r"(if_status));      \
                         asm("or %rax,(%rsp);                       \
                              popfq;                                \
                              pop %rax;                             \
                             ");
					
#define RET_FROM_INT_HANDLER_FLUSH asm("		                        \
                    movq %rbp,%rsp;	                                \
                    popq %rbp;                                      \
                    sub $4,%rbp,%rbp;	                            \	                                \
                    iret;		                                    \
					");

#define HALT asm("sti;hlt");
//#define SWITCH_PAGE_DIR(page_dir) asm("mov $0xe00000,%rax;mov %rax,%cr3;");
#define SWITCH_PAGE_DIR(page_dir) asm("mov %0,%%rax;mov %%rax,%%cr3;"::"r"(page_dir):"%rax");
//#define SWITCH_PAGE_DIR(page_dir) asm("mov %0,%%rax;"::"r"(page_dir):"%rax");
#define SYSCALL(syscall_num,params) asm("mov %0,%%rax;mov %1,%%rcx;int $0x80"::"r"(syscall_num),"r"(params):"%rax","%rcx");
#define SUSPEND asm("movq $0x65,%%rax;int $0x80":::"%rax","%rcx");
#define DO_STACK_FRAME(esp) asm("movq %0,%%rsp;"::"r"(esp));

unsigned char in(unsigned int address);
void out(unsigned char value,unsigned int address);
unsigned short inw(unsigned int address);
void outw(unsigned short value,unsigned int address);
unsigned int indw(u32 address);
void outdw(u32 value,u32 address);
void switch_to_user_mode();
void adjust_kernel_stack(unsigned int,unsigned int);
void syscall(unsigned int syscall_num,int* params);

#endif
