#ifndef MEM_REGS_H                
#define MEM_REGS_H

#include "system.h"

typedef struct s_mem_reg
{ 
	u64 start_addr;
	u64 end_addr;
	
} 
t_mem_reg;

t_mem_reg* create_mem_reg(u64 start_addr, u64 end_addr);
void delete_mem_reg(t_mem_reg* mem_reg);

#endif

