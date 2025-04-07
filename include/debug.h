#ifndef DEBUG_H                
#define DEBUG_H

#include "system.h"

void check_free_mem();
void check_process_context();
void check_active_process();
void collect_mem_alloc(u64 page_addr);
void collect_mem_free(u64 page_addr);
void reg_time(long long *timeboard,int* counter);
void _check_process_context(int val);
void panic();

#endif                                
