#ifndef BUDDY_H                
#define BUDDY_H

#include "system.h"
#include "data_types/primitive_types.h"
#include "memory_manager/general.h"

#define BLOCK_INDEX(mem_addr) (((mem_addr) - (BUDDY_START_ADDR)) / PAGE_SIZE)
//#define BLOCK_INDEX_FROM_PHY(virt_mem_addr)  BLOCK_INDEX((FROM_VIRT_TO_PHY(virt_mem_addr)))

struct s_buddy_desc 
{
	t_llist* page_list[11];
	char order[BUDDY_MEM_SIZE/PAGE_SIZE];
	void* page_list_ref[BUDDY_MEM_SIZE/PAGE_SIZE];
	short count[BUDDY_MEM_SIZE/PAGE_SIZE];
	unsigned int free_mem_list[11];
	t_hashtable* page_desc;
	t_spinlock_desc mp_lock;
}
typedef t_buddy_desc;

t_buddy_desc* buddy_init();
void* buddy_alloc_page(t_buddy_desc* buddy, u64 mem_size);
void buddy_free_page(t_buddy_desc* buddy, void* page_addr);
void buddy_clean_mem(void* page_addr);
u64 buddy_free_mem(t_buddy_desc* buddy);
void buddy_check_mem_status(t_buddy_desc* buddy);
void buddy_check_mem(t_buddy_desc* buddy, u64 mem_addr_to_check);

#endif

