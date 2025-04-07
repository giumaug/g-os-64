#ifndef MALLOC_H                
#define MALLOC_H

#define NULL 0
#define POOL_NUM 16
#define HEAP_VIRT_MEM_START_ADDR 0x200000000LL
#define MEM_TO_POOL 0x200000LL

typedef struct s_us_page_desc
{
	unsigned int block_free;
}
t_us_page_desc;

typedef struct s_us_block_desc
{
	struct s_us_block_desc *previous_block;
	struct s_us_block_desc *next_block;
}
t_us_block_desc;

typedef struct s_a_usr_space_desc
{
	t_us_block_desc *first_block;
        unsigned int has_initialized;
        unsigned int block_size;
        unsigned int current_free_block;
}
t_a_usr_space_desc;

void* _malloc(unsigned int mem_size);
void _free(void *address);

#endif

