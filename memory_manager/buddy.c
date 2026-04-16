#include "memory_manager/buddy.h"
#include "asm.h"
#include "virtual_memory/vm.h"

extern unsigned int collect_mem;
static unsigned int mem;

static void buddy_reset_block(void* address,unsigned int page_size);
static void buddy_init_mem(t_buddy_desc* buddy);
static void fill_buddy(t_buddy_desc* buddy, unsigned int num_list, u64 mem_addr, unsigned int num_block);
void buddy_check_mem(t_buddy_desc* buddy, u64 mem_addr_to_check);

t_buddy_desc* buddy_init()
{
	t_buddy_desc* buddy = NULL;
	u64 mem_addr;
	unsigned int num_entry;
	void* page_addr = NULL;
	u64 max_page_size;
	unsigned int align_offset;
	u64* mem_addr_bucket = NULL;
	int i,j;
	
	u64 _allocated_mem = ALLOCATED_MEM; 
    u64 _pool_start_addr = POOL_START_ADDR; 
    u64 _pool_end_addr = POOL_END_ADDR;
    u64 _mem_to_pool = MEM_TO_POOL; 
    u64 _buddy_start_addr = BUDDY_START_ADDR;
    u64 _buddy_end_addr = BUDDY_END_ADDR; 
    u64 _buddy_mem_size = BUDDY_MEM_SIZE;
    u64 _t_buddy_desc_size = sizeof(t_buddy_desc);
    u64 _t_buddy_desc_array_size = BUDDY_MEM_SIZE/PAGE_SIZE;
    
	buddy = VIRT_MEM_START_ADDR - PHY_MEM_START_ADDR + STATIC_BUDDY_STRUCT_START_ADDR;
	mem_addr = BUDDY_START_ADDR;
	max_page_size = MAX_PAGE_SIZE;
	
	SPINLOCK_INIT(buddy->mp_lock);
	for (i = 0; i < NUM_LIST; i++) 
	{
		buddy->page_list[i] = new_dllist();
	}
	num_entry = BUDDY_MEM_SIZE / max_page_size;
	for (j = 0; j < num_entry; j++) 
	{
		buddy->order[BLOCK_INDEX(mem_addr)] = NUM_LIST - 1;
		mem_addr_bucket = kmalloc(sizeof(u64));
		
		*mem_addr_bucket = mem_addr;			
		ll_prepend(buddy->page_list[NUM_LIST - 1], mem_addr_bucket);
		buddy->page_list_ref[BLOCK_INDEX(mem_addr)] = 0;
		buddy->count[BLOCK_INDEX(mem_addr)] = 0;
		mem_addr += max_page_size;
		//AHCI + 8254x memory
		if (mem_addr == 0xF1000000)
		{
			fill_buddy(buddy, 4, 0xF1010000, 1);
			fill_buddy(buddy, 0, 0xF1022000, 14);
			mem_addr = 0xF1030000;
		}
		//Video memory
		else if (mem_addr == 0xF0000000) 
		{
			fill_buddy(buddy, 0, 0xF0258000, 8);
			fill_buddy(buddy, 1, 0xF0260000, 10);
			fill_buddy(buddy, 9, 0xF0300000, 1);
			mem_addr = 0xF0300000;
		}
		//APIC + LAPIC memory
		else if (mem_addr == 0xFEC00000)
		{
			fill_buddy(buddy, 0, 0xFEC01000, 15);
			fill_buddy(buddy, 4, 0xFEC10000, 15);
			fill_buddy(buddy, 8, 0xFED00000, 1);
			
			fill_buddy(buddy, 0, 0xFEE01000, 15);
			fill_buddy(buddy, 4, 0xFEE10000, 15);
			fill_buddy(buddy, 8, 0xFEF00000, 1);
			mem_addr = 0xFF000000;
		}
	}
	return buddy;
}

static void fill_buddy(t_buddy_desc* buddy, unsigned int num_list, u64 mem_addr, unsigned int num_block)
{
	int i;
	u64 block_size;
	u64* mem_addr_bucket = NULL;
	
	block_size = PAGE_SIZE * (1 << num_list);
	for (i = 0; i < num_block; i++)
	{
		buddy->order[BLOCK_INDEX(mem_addr)] = num_list;
		mem_addr_bucket = kmalloc(sizeof(u64));
		*mem_addr_bucket = mem_addr;			
		ll_prepend(buddy->page_list[num_list], mem_addr_bucket);
		buddy->page_list_ref[BLOCK_INDEX(mem_addr)] = 0;
		buddy->count[BLOCK_INDEX(mem_addr)] = 0;
		mem_addr += block_size;
	}
}
	
void* buddy_alloc_page(t_buddy_desc* buddy, u64 mem_size)
{
	void* page_addr = NULL;	
	t_llist_node* node = NULL;
	t_llist_node* node_buddy = NULL;
	unsigned int next_list_index;
	unsigned int list_index = 1;
	unsigned int list_found = 0;
	u64 page_size;
	u64* mem_addr_bucket = NULL;
	void* new_mem_addr = NULL;
	int i;
	int y;
	
	SAVE_IF_STATUS
	CLI
	SPINLOCK_LOCK(buddy->mp_lock,get_current_process_context());
	for (list_index = 0; list_index < NUM_LIST; list_index++) 
	{	
		if (PAGE_SIZE * (1 << list_index) >= mem_size) break;
	}
	next_list_index = list_index;
	while (next_list_index < NUM_LIST && !list_found)
	{
		if (!ll_empty(buddy->page_list[next_list_index]))
		{
        	node = ll_first(buddy->page_list[next_list_index]);
			list_found = 1;
		}
		else 
		{
			next_list_index++;
		}
	}
	if (list_found == 0)
	{
		printk("run out of buddy memory!!!");
		panic();
	}
	page_addr =*(u64*)(node->val);
	kfree(node->val);
	ll_delete_node(node);
	if (next_list_index != list_index)
	{
		page_size = PAGE_SIZE * (1 << next_list_index);
        for ( i = (next_list_index-1); i >= (int)list_index; i--)
		{
			page_size /= 2;
			mem_addr_bucket = kmalloc(sizeof(u64));
			*mem_addr_bucket = page_addr + page_size;
			node_buddy = ll_prepend(buddy->page_list[i], mem_addr_bucket);
			buddy->order[BLOCK_INDEX((u64)page_addr + page_size)] = i;
			buddy->page_list_ref[BLOCK_INDEX((u64)page_addr + page_size)] = node_buddy;
			system.buddy_desc->count[BLOCK_INDEX((u64)page_addr + page_size)] = 0;
		}
	}
	buddy->order[BLOCK_INDEX((u64)page_addr)] = (list_index | 16);
	buddy->page_list_ref[BLOCK_INDEX((u64)page_addr)] = node;
	system.buddy_desc->count[BLOCK_INDEX((u64)page_addr)] = 0;
	new_mem_addr = page_addr + VIRT_MEM_START_ADDR - PHY_MEM_START_ADDR;
		
	if (collect_mem == 1)
	{
		collect_mem_alloc(new_mem_addr);
	}
	SPINLOCK_UNLOCK(buddy->mp_lock,get_current_process_context());
	RESTORE_IF_STATUS
	return new_mem_addr;
}

void buddy_free_page(t_buddy_desc* buddy,void* to_free_page_addr)
{
	unsigned int stop_coalesce;	
	unsigned int list_index;
	unsigned int page_size;
	unsigned int near_index;
    u64 page_addr;
	u64 buddy_page_addr; 
	u64 free_page_addr;
	unsigned int free_page_order;
	u64* mem_addr_bucket = NULL;
	t_llist_node* node = NULL;
	t_llist_node* node_buddy = NULL;
	int i;
	
	SAVE_IF_STATUS
	CLI
	SPINLOCK_LOCK(buddy->mp_lock,get_current_process_context());
	page_addr = to_free_page_addr;
	
	if (collect_mem == 1)
	{
		collect_mem_free(page_addr);
	}

	page_addr = page_addr - VIRT_MEM_START_ADDR + PHY_MEM_START_ADDR;
	
	if (page_addr == 0xf026efff)
	{
		panic();
	}
	
	
	if ((buddy->order[BLOCK_INDEX(page_addr)] & 16) == 0)
	{
		panic();
	}
	list_index = buddy->order[BLOCK_INDEX(page_addr)] & 15;
	free_page_addr = page_addr;
	free_page_order = list_index;
	stop_coalesce = 0;	
	i = list_index;
	
	while(i < NUM_LIST - 1 && !stop_coalesce)
	{
		page_size = PAGE_SIZE * (1 << i);
        buddy_page_addr = page_addr ^ page_size;
		if ((buddy->order[BLOCK_INDEX(buddy_page_addr)] & 15)== i && 
		   (buddy->order[BLOCK_INDEX(buddy_page_addr)] & 16) == 0 && 
		    buddy->page_list_ref[BLOCK_INDEX((buddy_page_addr))] != NULL)
		{
			node = buddy->page_list_ref[BLOCK_INDEX((buddy_page_addr))];
			kfree(node->val);			
			ll_delete_node(node);
			buddy->order[BLOCK_INDEX(buddy_page_addr)] = 0;
			buddy->page_list_ref[BLOCK_INDEX((buddy_page_addr))] = NULL;
			page_addr &= buddy_page_addr;
			free_page_addr = page_addr;
			free_page_order = i + 1;
			page_addr &= buddy_page_addr;
			buddy->order[BLOCK_INDEX(page_addr)] = buddy->order[BLOCK_INDEX(page_addr)] & 15;
		}
		else 
		{
			stop_coalesce = 1;
		}
		i++;
	}
	mem_addr_bucket = kmalloc(sizeof(u64));
	*mem_addr_bucket = free_page_addr;
	node_buddy = ll_prepend(buddy->page_list[free_page_order], mem_addr_bucket);
	buddy->page_list_ref[BLOCK_INDEX(free_page_addr)] = node_buddy;
	buddy->order[BLOCK_INDEX(free_page_addr)] = free_page_order;
	SPINLOCK_UNLOCK(buddy->mp_lock,get_current_process_context());
	RESTORE_IF_STATUS
}

u64 buddy_free_mem(t_buddy_desc* buddy_desc)
{
	int i;
	u64 tot;
	u64 tmp;
	
	tot = 0;
	for (i = 0; i < 11; i++)
	{	
		tmp = ll_size(buddy_desc->page_list[i]);
		buddy_desc->free_mem_list[i] = tmp;
		tot = tot +PAGE_SIZE * ( 1 << i) * tmp;
		if (i == 10)
		{
			//printk("index =:%d \n",i);
			printk("  slots:%d \n",tmp);
		}
	}
	return tot;
}

void buddy_clean_mem(void* page_addr)
{
	int i;

	for ( i = 0; i < PAGE_SIZE / 8; i++)
	{
		((u64*)page_addr)[i] = 0;
	}
	if (i == 666)
	{
		panic();
	}
}

static void buddy_init_mem(t_buddy_desc* buddy)
{
	t_llist* list = NULL;
	t_llist_node* next = NULL;
	t_llist_node* sentinel = NULL;
	u64* val = NULL;
	unsigned int i;
	unsigned int y;
	u64 page_size;
	t_llist_node* next_tmp = NULL;
	unsigned char* mem_addr;

	for ( i = 0; i < NUM_LIST; i++)
	{
		page_size = PAGE_SIZE * (1 << i);
		list = buddy->page_list[i];
		sentinel = ll_sentinel(list);
		next = ll_first(list);
		next_tmp = ll_next(next);
		while(next != sentinel)
		{
			val = next->val;
			mem_addr = BUDDY_START_ADDR + VIRT_MEM_START_ADDR + (u64) *val;
			for ( y = 0; y < page_size; y++)
			{
				*(mem_addr + y) = 0xFF;
			}	
			next=ll_next(next);
		}
	}
}

void buddy_check_mem_status(t_buddy_desc* buddy)
{
	t_llist* list = NULL;
	t_llist_node* next = NULL;
	t_llist_node* sentinel = NULL;
	u64* val = NULL;
	unsigned int i;
	unsigned int y;
	u64 page_size;
	unsigned char* mem_addr;

	for (i = 0; i < NUM_LIST; i++)
	{	
		page_size = PAGE_SIZE * (1 << i);
		list = buddy->page_list[i];
		sentinel = ll_sentinel(list);
		next = ll_first(list);
		while (next != sentinel)
		{
			val = next->val;
			mem_addr = BUDDY_START_ADDR + VIRT_MEM_START_ADDR+(u64) *val;
			for ( y = 0; y < page_size; y++)
			{
				if (*(mem_addr + y) != 0xFF)
				{
					panic();
				}
			}	
			next = ll_next(next);
		}
	}
}

static void buddy_reset_block(void* address,unsigned int page_size)
{
	int i;
	
	for (i = 0; i < page_size; i++)
	{
		*(unsigned char*)(address+i) = 0xFF;
	}
}

void buddy_check_mem(t_buddy_desc* buddy, u64 mem_addr_to_check)
{
	int counter = 0;
	
	t_llist* list = NULL;
	t_llist_node* next = NULL;
	t_llist_node* sentinel = NULL;
	u64* val = NULL;
	unsigned int i;
	unsigned int y;
	u64 page_size;
	t_llist_node* next_tmp = NULL;
	u64 mem_addr;

	for ( i = 0; i < NUM_LIST; i++)
	{
		page_size = PAGE_SIZE * (1 << i);
		list = buddy->page_list[i];
		sentinel = ll_sentinel(list);
		next = ll_first(list);
		next_tmp = ll_next(next);
		while(next != sentinel)
		{
			val = next->val;
			mem_addr = (u64) *val;
			if (mem_addr_to_check == mem_addr)
			{
				//panic();
				counter++;
			}
			next=ll_next(next);
		}
	}
	if (counter > 1)
	{
		panic();
	}
}
