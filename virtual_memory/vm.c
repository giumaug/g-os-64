#include "lib/lib.h"
#include "idt.h"
#include "asm.h"
#include "memory_manager/general.h"
#include "scheduler/process.h"
#include "virtual_memory/vm.h"

static s32 pages_index = -PAGE_SIZE;

void page_fault_handler();

void ret_from_int_handler_flush();


void* init_virtual_memory()
{
	static struct t_i_desc i_desc;
	u32 i,y,z;
	u32 dir_num;
	//Still with temp memory mapping. Physical and virtual table pages coincide.
	u64* pml4 = PHY_MASTER_PML4;
	u64* ptr = PHY_MASTER_PTR;
	u64* dir = PHY_MASTER_DIR;
	u64* pages = PHY_MASTER_PAGES;
	u64 mem_addr = 0;
	u8* static_mem = NULL;
	u32 static_mem_size;

	i_desc.baseLow=(((u64)(&page_fault_handler)) & 0xFFFF);
	i_desc.selector=0x8;
	i_desc.flags=0x08e00;
	i_desc.baseHi=(((u64)(&page_fault_handler)) >> 0x010);
	i_desc.baseExt=(((u64)(&page_fault_handler)) >> (u64)0x020);
	i_desc.pad=0;
	set_idt_entry(0x0E,&i_desc);
	
	static_mem_size = STATIC_BUDDY_STRUCT_START_ADDR -  PHY_MASTER_PML4;
	static_mem = PHY_MASTER_PML4;
	for (i = 0; i < static_mem_size; i++)
	{
		static_mem[i] = NULL;
	}
	
	pml4[0] = ((u64) ptr) | PAGE_IN_MEMORY | PAGE_WRITE;
	ptr[0] = ((u64) dir) | PAGE_IN_MEMORY | PAGE_WRITE;
	
	//Map 1M - end page table memory address
	for (y = 0; y < 2; y++)
    {
		pages_index += PAGE_SIZE;
		dir[y] = ((u64) pages) + pages_index | PAGE_IN_MEMORY | PAGE_WRITE | USER;
		for(z = 0; z < 512; z++)
		{
			pages[((pages_index / 8) + z)] = mem_addr | PAGE_IN_MEMORY | PAGE_WRITE | SUPERUSER;
			mem_addr += PAGE_SIZE;
		}
	}	
	
	//Map kernel memory
	mem_addr = PHY_MEM_START_ADDR;
    for (y = 1; y < (G_PHY_MEM_SIZE + 1); y++)
    {
		ptr[15 + y] =  ((u64) dir) + (y * 4096) | PAGE_IN_MEMORY | PAGE_WRITE | USER;
		for (i = 0; i < 512; i++)
		{
			pages_index += PAGE_SIZE;
			dir[i + (512 * y)] = ((u64) pages) + pages_index | PAGE_IN_MEMORY | PAGE_WRITE | USER;
			for(z = 0; z < 512; z++)
			{
				pages[((pages_index / 8) + z)] = mem_addr | PAGE_IN_MEMORY | PAGE_WRITE | SUPERUSER;
				mem_addr += PAGE_SIZE;
			}
		}
	}
	return FROM_PHY_TO_VIRT(((u64)pml4));
}

void init_vm_process(struct t_process_context* process_context)
{
	u32 i,y;
	u64* page_pml4 = NULL;
	u64* page_ptr = NULL;
	u64* page_dir =  NULL;
	u64* page_table = NULL;
	u64* master_page_ptr = NULL;
	struct t_process_context* current_process_context = NULL;
	u32 dir_num = 0;
	
	page_pml4 = buddy_alloc_page(system.buddy_desc,0x1000);
	buddy_clean_mem(page_pml4);
	process_context->page_pml4 = page_pml4;
	
	page_ptr = buddy_alloc_page(system.buddy_desc,0x1000);
	buddy_clean_mem(page_ptr);
	((u64*)page_pml4)[0] = FROM_VIRT_TO_PHY((u64) page_ptr) | 7;
	
	master_page_ptr = ALIGN_4K(FROM_PHY_TO_VIRT(((u64*)system.master_page_pml4)[0]));
	for (i = 0; i < 1; i++)
    {
		page_ptr[i] = master_page_ptr[i];
	}
		
	for (i = 16; i < 16 + G_PHY_MEM_SIZE; i++)
	{
		page_ptr[i] = master_page_ptr[i];
	}
	
	map_vm_mem(page_pml4, (KERNEL_STACK-KERNEL_STACK_SIZE), process_context->phy_kernel_stack, KERNEL_STACK_SIZE, 3);
	SWITCH_PAGE_DIR(FROM_VIRT_TO_PHY((((u64)process_context->page_pml4))))
}
	
void* clone_vm_process(void* parent_page_pml4,u64 process_type, u64 kernel_stack_addr)
{
	int i,j,z,dir_num;
	u64* page_pml4 = NULL;
	u64* page_ptr = NULL;
	u64* page_dir =  NULL;
	u64* page_table = NULL;
	u64* parent_page_ptr = NULL;
	u64* parent_page_dir =  NULL;
	u64* parent_page_table = NULL;
	struct t_process_context* current_process_context = NULL;
	
	SAVE_IF_STATUS
	CLI
	page_pml4 = buddy_alloc_page(system.buddy_desc,0x1000);
	buddy_clean_mem(page_pml4);
	
	page_ptr = buddy_alloc_page(system.buddy_desc,0x1000);
	buddy_clean_mem(page_ptr);
	page_pml4[0] = FROM_VIRT_TO_PHY((u64)page_ptr) | 7;
	
	parent_page_ptr = ALIGN_4K(FROM_PHY_TO_VIRT(((u64*)parent_page_pml4)[0]));	
	map_vm_mem(page_pml4, (KERNEL_STACK-KERNEL_STACK_SIZE), kernel_stack_addr, KERNEL_STACK_SIZE, 3);
	if (process_type == USERSPACE_PROCESS)
	{
		for (i = 1; i <= 15; i++)
		{
			if (parent_page_ptr[i] != 0)
			{
				parent_page_dir = ALIGN_4K(FROM_PHY_TO_VIRT(((u64*)parent_page_ptr)[i]));
				if (i != 15)
				{
					page_dir = buddy_alloc_page(system.buddy_desc,0x1000);
					buddy_clean_mem(page_dir);
					page_ptr[i] = FROM_VIRT_TO_PHY((u64)page_dir) | 7;
				}
				else
				{
					page_dir = ALIGN_4K(FROM_PHY_TO_VIRT(((u64*)page_ptr)[i]));
				}
				for (j = 0; j < 512 ;j++)
				{
					if (parent_page_dir[j] != 0)
					{
						parent_page_table = ALIGN_4K(FROM_PHY_TO_VIRT(((u64*)parent_page_dir)[j]));
						if (i != 15 && j != 510)
						{				
							page_table = buddy_alloc_page(system.buddy_desc,PAGE_SIZE);
							buddy_clean_mem(page_table);
							page_dir[j] = FROM_VIRT_TO_PHY(((u64) page_table)) | 7;
						}
						else
						{
							page_table = ALIGN_4K(FROM_PHY_TO_VIRT(((u64*) page_dir)[510]));
						}
								
						for (z = 0; z < 512; z++)
						{ 	
							if (i !=15 || (i == 15 && j !=510) || (i == 15 && j ==510 && z != 253 && z != 254))
							{
								if (parent_page_table[z] != 0)
								{
									system.buddy_desc->count[BLOCK_INDEX(ALIGN_4K(parent_page_table[z]))]++;
									parent_page_table[z] &= 0xFFFFFFFFFFFFFFFD;
									page_table[z] = parent_page_table[z];
								}	
							}
						}
					}
				}
			}		
		}
	}
		
	page_ptr[0] = parent_page_ptr[0];
	for (i = 16; i < 16 + G_PHY_MEM_SIZE; i++)
	{
		page_ptr[i] = parent_page_ptr[i];
	}	
	CURRENT_PROCESS_CONTEXT(current_process_context);
	SWITCH_PAGE_DIR(FROM_VIRT_TO_PHY((current_process_context->page_pml4))) 	
	RESTORE_IF_STATUS
	return page_pml4;		
}
	
void free_vm_process(struct t_process_context* process_context)
{
	u64* page_pml4 = NULL;
	u64* page_ptr = NULL;
	
	SAVE_IF_STATUS
	CLI
	page_pml4 = process_context->page_pml4;
	page_ptr = ALIGN_4K(FROM_PHY_TO_VIRT(((u64*)page_pml4)[0]));
	if (process_context->process_type==USERSPACE_PROCESS)
	{
		free_vm_process_user_space(process_context);
	}
	//umap_vm_mem(page_pml4, 0, 0x100000, 1);
	umap_vm_mem(page_pml4, KERNEL_STACK, KERNEL_STACK_SIZE, 1);
	buddy_free_page(system.buddy_desc,page_pml4);
	buddy_free_page(system.buddy_desc,page_ptr);
	RESTORE_IF_STATUS
}
	
void free_vm_process_user_space(struct t_process_context* process_context)
{
	unsigned int i,j, z;
	u64* page_pml4 = NULL;
	u64* page_ptr = NULL;
	u64* page_dir =  NULL;
	u64* page_table = NULL;
	
	SAVE_IF_STATUS
	CLI
	page_pml4 = process_context->page_pml4;
	page_ptr = ALIGN_4K(FROM_PHY_TO_VIRT(((u64*)page_pml4)[0]));
	//page_dir = ALIGN_4K(FROM_PHY_TO_VIRT(((u64*)page_ptr)[0]));
	//page_table = ALIGN_4K(FROM_PHY_TO_VIRT(((u64*)page_dir)[0]));
		
	for (i = 1; i <= 15; i++)
	{
		if (page_ptr[i] != 0)
		{
			page_dir = ALIGN_4K(FROM_PHY_TO_VIRT(((u64*)page_ptr)[i]));
			for (j = 0; j < 512 ;j++)
			{
				if (page_dir[j] != 0)
				{					
					page_table= ALIGN_4K(FROM_PHY_TO_VIRT(((u64*)page_dir)[j]));
					for (z = 0; z < 512; z++) 
					{	
						if (i !=15 || (i == 15 && j !=510) || (i == 15 && j ==510 && z != 253 && z != 254))
						{
							if (page_table[z] != 0 && system.buddy_desc->count[BLOCK_INDEX(ALIGN_4K(page_table[z]))] == 1)
							{
								u64 uuu = BLOCK_INDEX(ALIGN_4K(page_table[z]));
								u64 ooo = ALIGN_4K(page_table[z]);
								u64 sss = BUDDY_START_ADDR;
								buddy_free_page(system.buddy_desc,ALIGN_4K(FROM_PHY_TO_VIRT(page_table[z])));
								system.buddy_desc->count[BLOCK_INDEX(ALIGN_4K(page_table[z]))] = 0;
								page_table[z] = 0;
							}
							else if (page_table[z] != 0)
							{
								system.buddy_desc->count[BLOCK_INDEX(ALIGN_4K(page_table[z]))]--;
								page_table[z] = 0;
							}
					    }
					}
					if (i != 15 || (i == 15 && j != 510))
					{
						buddy_free_page(system.buddy_desc, page_table);
						page_dir[j]=0;
					}
				}
			}	
			if (i != 15)
			{
				buddy_free_page(system.buddy_desc, page_dir);
				page_ptr[i]=0;
			}
		}
	}
	RESTORE_IF_STATUS
}

//Map and umap manage 128G memory with address in the same giga only.	
int map_vm_mem(u64* page_pml4, u64 vir_mem_addr, u64 phy_mem_addr, u64 mem_size, u32 flags)
{
	unsigned int i, j, dir_num;
	u64* page_ptr = NULL;
	u64* page_dir =  NULL;
	u64* page_table = NULL;
	u64 start,end;
	u64 page_count;
	u64 pd_count;
	u64 first_pd;
	u64 first_pt;
	u64 last_pt;
	u64 last_pd;
	int ret = -1;
	
	if (((vir_mem_addr & 0x7FC0000000) >> 30) == (((vir_mem_addr + mem_size) & 0x7FC0000000) >> 30))
	{
		dir_num = (vir_mem_addr & 0x7FC0000000) >> 30;
		page_ptr = ALIGN_4K(FROM_PHY_TO_VIRT(page_pml4[0]));
		page_dir = ALIGN_4K(FROM_PHY_TO_VIRT(page_ptr[dir_num]));
		
		if (((u64*)(page_ptr))[dir_num] == 0)
		{	
			page_dir = buddy_alloc_page(system.buddy_desc, 0x1000);
			buddy_clean_mem(page_dir);
			//ENABLE TO ALL PAGE DIR TO MANAGE PAGE DIR WITH PAGE TABLE WITH DIFFERENT PRIVILEGES
			((u64*)page_ptr)[dir_num] = FROM_VIRT_TO_PHY((u64)page_dir) | 7;
		}
		else 
		{
			page_dir = ALIGN_4K(FROM_PHY_TO_VIRT(((u64*)page_ptr)[dir_num]));
		}
		
		phy_mem_addr -= 4096;
		page_count = mem_size / 4096;
		if ((mem_size % 4096) > 0) page_count++;
		pd_count = page_count / 512;
		if ((page_count % 512) > 0) pd_count++;
		first_pd = (vir_mem_addr & 0x3FFFFFFF)  >> 21;
		first_pt = (vir_mem_addr & 0x1FFFFF) >> 12;
		last_pt = ((vir_mem_addr + mem_size - 1) & 0x1FFFFF) >> 12;
		last_pd = pd_count + first_pd;
		
		for (i = first_pd; i < last_pd; i++)
		{
			if (((u64*)(page_dir))[i] == 0)
			{	
				page_table = buddy_alloc_page(system.buddy_desc, 0x1000);
				buddy_clean_mem(page_table);
				//ENABLE TO ALL PAGE DIR TO MANAGE PAGE DIR WITH PAGE TABLE WITH DIFFERENT PRIVILEGES
				((u64*)page_dir)[i] = FROM_VIRT_TO_PHY((u64)page_table) | 7;
			}
			else 
			{
				page_table = ALIGN_4K(FROM_PHY_TO_VIRT(((u64*)page_dir)[i]));
			}
			if (i == first_pd && pd_count > 1) 
			{
				start = first_pt;
				end = 512;
			}
			else if (i == first_pd && pd_count == 1) 
			{
				start = first_pt;
				end = last_pt + 1;
			}
			else if (i == last_pd - 1)
			{
				start = 0;
				end = last_pt + 1;
			}
			else 
			{
				start = 0;
				end = 512;
			}
		
			for (j = start; j < end; j++)
			{
				phy_mem_addr = (phy_mem_addr + 4096) | flags;
				page_table[j] = phy_mem_addr;
			}
		}
		ret = 0;
	}
	else
	{
		panic();
	}
	return ret;
}
	
int umap_vm_mem(u64* page_pml4, u64 vir_mem_addr, u64 mem_size, u32 flush)
{
	unsigned int i, j, dir_num;
	u64* page_ptr = NULL;
	u64* page_dir =  NULL;
	u64* page_table = NULL;
	u64 start,end;
	u64 page_count;
	u64 pd_count;
	u64 first_pd;
	u64 first_pt;
	u64 last_pt;
	u64 last_pd;
	int free_page = 0;
	int ret = -1;
	
	if (((vir_mem_addr & 0x7FC0000000) >> 30) == (((vir_mem_addr + mem_size) & 0x7FC0000000) >> 30))
	{
		//dir_num = vir_mem_addr / G_PHY_MEM_SIZE;
		dir_num = (vir_mem_addr & 0x7FC0000000) >> 30;
		page_ptr = ALIGN_4K(FROM_PHY_TO_VIRT(page_pml4[0]));
		page_dir = ALIGN_4K(FROM_PHY_TO_VIRT(page_ptr[dir_num]));
		
		page_count = mem_size / 4096;
		if ((mem_size % 4096) > 0) page_count++;
		pd_count = page_count / 512;
		if ((page_count % 512) > 0) pd_count++;
		first_pd = (vir_mem_addr & 0x3FFFFFFF)  >> 21;
		first_pt = (vir_mem_addr & 0x1FFFFF) >> 12;
		last_pt = ((vir_mem_addr + mem_size - 1) & 0x1FFFFF) >> 12;
		last_pd = pd_count + first_pd;
		
		for (i = first_pd; i < last_pd; i++)
		{	
			page_table = ALIGN_4K(FROM_PHY_TO_VIRT(page_dir[i]));   

			if (i == first_pd && last_pd > 1) 
			{
			 start = first_pt;
				end = 512;
			}
			else if (i == first_pd && last_pd == 1) 
			{
				start = first_pt;
				end = last_pt + 1;
			}
			else if (i == last_pd - 1)
			{
				start = 0;
				end = last_pt + 1;
			}
			else 
			{
				start = 0;
				end = 512;
			}
			for (j = start; j < end; j++)
			{
				page_table[j] = 0;
			}
			
			free_page = 0;
			for (j = 0; j < 512; j++)
			{
				if (page_table[j] == 0)
				{
					free_page++;
				}
			}
			if (free_page == 512 || flush)
			{
				buddy_free_page(system.buddy_desc, page_table);
				((u64*)page_dir)[i] = 0;
			}
//			if ((start == 0 && end == 512) || flush) 
//			{
//				buddy_free_page(system.buddy_desc, page_table);
//				((u64*)page_dir)[i] = 0;
//			}
		}
		free_page = 0;
		for (j = 0; j < 512; j++)
		{
			if (page_dir[j] == 0)
			{
				free_page++;
			}
		}
		if (free_page == 512)
		{
			buddy_free_page(system.buddy_desc, page_dir);
			page_ptr[dir_num] = 0;
		}
		ret = 0;
	}
	else
	{
		panic();
	}
	return ret;
}
	
void page_fault_handler()
{
	u64* page_pml4 = NULL;
	u64* page_ptr = NULL;
	u64* page_dir =  NULL;
	u64* page_table = NULL;
	u64 on_exit_action;
	u64* ustack_pointer = NULL;
	u64 fault_addr;
	u64 aligned_fault_addr;
	char fault_code;
	u64 page_addr;
	u64 stack_reg_start;
	struct t_process_context* current_process_context;
	struct t_processor_reg processor_reg;
	u64 dir_num;
	u64 pd_num;
	u64 pt_num;
	u64 phy_fault_addr;
	u64 phy_page_addr;
	
	SAVE_PROCESSOR_REG(processor_reg)
	SWITCH_SS_TO_KERNEL_MODE
	GET_FAULT_ADDRESS(fault_addr, fault_code);
	CURRENT_PROCESS_CONTEXT(current_process_context);
	on_exit_action=0;
	aligned_fault_addr=fault_addr & (~(PAGE_SIZE-1));
	
	if (CHECK_MEM_REG(fault_addr, current_process_context->process_mem_reg)
		    || CHECK_MEM_REG(fault_addr, current_process_context->heap_mem_reg)
		    || CHECK_MEM_REG(fault_addr, current_process_context->ustack_mem_reg))
	{
		if ((fault_code & 0x1)==PAGE_OUT_MEMORY && CHECK_MEM_REG(fault_addr, current_process_context->process_mem_reg))
		{
			page_addr = 0;
			page_addr = buddy_alloc_page(system.buddy_desc, PAGE_SIZE);
			map_vm_mem(current_process_context->page_pml4, aligned_fault_addr, FROM_VIRT_TO_PHY(page_addr), PAGE_SIZE, 7);
			phy_page_addr = FROM_VIRT_TO_PHY(page_addr);
			system.buddy_desc->count[BLOCK_INDEX(phy_page_addr)]++;
			elf_loader_read(current_process_context->elf_desc, fault_addr, page_addr);
			printk(".");
		}
		else if ((fault_code & 0x1) == PAGE_OUT_MEMORY && (CHECK_MEM_REG(fault_addr, current_process_context->heap_mem_reg) || CHECK_MEM_REG(fault_addr, current_process_context->ustack_mem_reg)))
		{
			page_addr = buddy_alloc_page(system.buddy_desc, PAGE_SIZE);
			phy_page_addr = FROM_VIRT_TO_PHY(page_addr);
			map_vm_mem(current_process_context->page_pml4, aligned_fault_addr, FROM_VIRT_TO_PHY(page_addr), PAGE_SIZE, 7);
			system.buddy_desc->count[BLOCK_INDEX(phy_page_addr)]++;			
		}
		else if ((fault_code & 0x3) == (PAGE_IN_MEMORY | PAGE_WRITE))
		{
			dir_num = (aligned_fault_addr & 0x7FC0000000) >> 30;
			pd_num = (aligned_fault_addr & 0x3FFFFFFF) >> 21;
			pt_num = (aligned_fault_addr & 0x1FFFFF) >> 12;	
			page_pml4 = current_process_context->page_pml4;
			
			page_ptr = ALIGN_4K(FROM_PHY_TO_VIRT(((u64*)page_pml4)[0]));
		    page_dir = ALIGN_4K(FROM_PHY_TO_VIRT(((u64*)page_ptr)[dir_num]));
			page_table = ALIGN_4K(FROM_PHY_TO_VIRT(((u64*) page_dir)[pd_num]));
			
			phy_fault_addr = ALIGN_4K(((u64*) page_table)[pt_num]);
			((u64*) page_table)[pt_num] |= 7;
			if (system.buddy_desc->count[BLOCK_INDEX(phy_fault_addr)] > 1)
			{
				page_addr = buddy_alloc_page(system.buddy_desc,PAGE_SIZE);
				kmemcpy(page_addr, aligned_fault_addr, PAGE_SIZE);
				map_vm_mem(current_process_context->page_pml4, aligned_fault_addr, FROM_VIRT_TO_PHY(page_addr), PAGE_SIZE, 7);
				system.buddy_desc->count[BLOCK_INDEX(phy_fault_addr)]--;
				phy_page_addr = FROM_VIRT_TO_PHY(page_addr);
				system.buddy_desc->count[BLOCK_INDEX(phy_page_addr)]++;
			}
		}
		else
		{
			printk("Unexpected path!!! \n");
		}
	}
    //*processor_reg.esp+20 is the user stack pointer.This case manages user stack enlargment.
	else if ((fault_code & 0x4)==USER && ((*(u64*)(processor_reg.rsp+20))-32)<=fault_addr)
	{
		current_process_context->ustack_mem_reg->start_addr=aligned_fault_addr;
		page_addr=buddy_alloc_page(system.buddy_desc,PAGE_SIZE);
		map_vm_mem(current_process_context->page_pml4,aligned_fault_addr,FROM_VIRT_TO_PHY(page_addr),PAGE_SIZE,7);
		phy_page_addr = FROM_VIRT_TO_PHY(page_addr);
		system.buddy_desc->count[BLOCK_INDEX(phy_page_addr)]++;
	}
	else
	{
		//printk("\n +++Segmentation fault. \n");
		panic();
		//_exit(0);
		//on_exit_action=2;
		on_exit_action = 0;
		page_addr=buddy_alloc_page(system.buddy_desc,PAGE_SIZE);
		map_vm_mem(current_process_context->page_pml4,aligned_fault_addr,FROM_VIRT_TO_PHY(page_addr),PAGE_SIZE,7);
	}

	SWITCH_PAGE_DIR(FROM_VIRT_TO_PHY((current_process_context->page_pml4))) 	
//	EXIT_INT_HANDLER(on_exit_action,processor_reg)
    exit_int_handler(processor_reg, -1, NULL);
} 

