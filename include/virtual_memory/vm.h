#ifndef VM_H                
#define VM_H

#include "system.h"

#define VIRT_MEM_START_ADDR 0x400000000ULL
#define VIRT_MEM_END_ADDR 0x400000000ULL + PHY_MEM_SIZE
#define PHY_MEM_START_ADDR  0x100000ULL
#define PAGE_TABLE_ENTRY HIGH_MEM_LIMIT / 0x1000ULL
#define PAGE_DIR_ENTRY PAGE_TABLE_ENTRY / 0x400ULL
#define PROC_VIRT_MEM_START_ADDR 0x100000ULL    	//1M - 8G
#define HEAP_VIRT_MEM_START_ADDR 0x200000000ULL  	//8G - 16G
#define HEAP_INIT_SIZE 0x200000ULL
#define KERNEL_STACK 0x3FFCFF000ULL        			//16G - 3M - 4k hole; size = 0x2000
#define KERNEL_STACK_SIZE 0x2000ULL          
#define USER_STACK 0x3FFCFC000ULL                	//16G - 3M - 4K hole - 8K - 4K hole;size 0x40000000
#define USER_STACK_INIT_SIZE 0x40000000ULL

#define PAGE_IN_MEMORY 	0b001
#define PAGE_OUT_MEMORY  0b000
#define PAGE_READ 	    0b000
#define PAGE_WRITE	    0b010
#define USER		        0b100
#define SUPERUSER	    0b000    


#define FROM_VIRT_TO_PHY(addr) addr-VIRT_MEM_START_ADDR+PHY_MEM_START_ADDR
#define FROM_PHY_TO_VIRT(addr) addr+VIRT_MEM_START_ADDR-PHY_MEM_START_ADDR

#define CHECK_MEM_REG(fault_addr,mem_reg) fault_addr>=mem_reg->start_addr && fault_addr<=mem_reg->end_addr 
#define ALIGN_4K(address) ((u64)address & 0xFFFFFFFFFFFFF000LL)
#define ALIGNED_TO_OFFSET(address, offset) ((u64)address + offset) - ((u64)address % offset);

void* init_virtual_memory();
void init_vm_process(struct t_process_context* process_context);
void free_vm_process(struct t_process_context* process_context);
int map_vm_mem(u64* page_pml4, u64 vir_mem_addr, u64 phy_mem_addr, u64 mem_size, u32 flags);
int umap_vm_mem(u64* page_pml4, u64 virt_mem_addr, u64 mem_size, u32 flush);
void* clone_vm_process(void* parent_page_pml4,u64 process_type, u64 kernel_stack_addr);
void free_vm_process_user_space(struct t_process_context* process_context);

#endif



