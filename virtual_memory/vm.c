#include "lib/lib.h"
#include "idt.h"
#include "asm.h"
#include "memory_manager/general.h"
#include "scheduler/process.h"
#include "virtual_memory/vm.h"

void* init_virtual_memory()
{
	u32 i,y,z;
	u32 dir_num;
	u32 pages_index = 0;
	u64* pml4 = MASTER_PML4;
	u64* ptr = MASTER_PTR;
	u64* dir = MASTER_DIR;
	u64* pages = MASTER_PAGES;
	u64 mem_addr = 0;
	
	pml4[0] = ((u64) ptr) | PAGE_IN_MEMORY | PAGE_WRITE;
	ptr[0] = ((u64) dir) | PAGE_IN_MEMORY | PAGE_WRITE;
	
	//Map 0-1 MB memory
	dir[0] = ((u64) pages) | PAGE_IN_MEMORY | PAGE_WRITE;
	for(y = 0; y < 256; y++)
	{
		pages[(pages_index + y)] = mem_addr | PAGE_IN_MEMORY | PAGE_WRITE;
		mem_addr += PAGE_SIZE;
	}
	pages_index += PAGE_SIZE;
	
	//Map 16M - 1G memory
	mem_addr = PHY_MEM_START_ADDR;
    dir_num = PHY_MEM_SIZE / 0x40000000;
    for (y = 1; y < (dir_num + 1); y++)
    {
		ptr[15 + y] =  ((u64) dir) + (y * 4096) | PAGE_IN_MEMORY | PAGE_WRITE;
		
		for (i = 0; i < 512; i++)
		{
			dir[i + (512 * y)] = ((u64) pages) + pages_index | PAGE_IN_MEMORY | PAGE_WRITE;
			for(z = 0; z < 512; z++)
			{
				pages[((pages_index / 8) + z)] = mem_addr | PAGE_IN_MEMORY | PAGE_WRITE;
				mem_addr += PAGE_SIZE;
			}
			pages_index++;
		}
	}
	return pml4;
}
	
void init_vm_process(struct t_process_context* process_context)
{
	
}
	
void* clone_vm_process(void* parent_page_dir,u32 process_type,u32 kernel_stack_addr)
{

}
	
void free_vm_process(struct t_process_context* process_context)
{

}
	
void free_vm_process_user_space(struct t_process_context* process_context)
{

}
	
void map_vm_mem(void* page_dir,unsigned int vir_mem_addr,unsigned int phy_mem_addr,int mem_size,u32 flags)
{

}
	
void umap_vm_mem(void* page_dir,unsigned int virt_mem_addr,unsigned int mem_size,unsigned int flush)
{

}
	
void page_fault_handler()
{

} 

