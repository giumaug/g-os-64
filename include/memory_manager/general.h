#ifndef MEMORY_GENERAL_H                
#define MEMORY_GENERAL_H

#include "system.h"

//Sytem memory must be in [1G-16G] range.

#define ALIGNED_TO_OFFSET(address, offset) ((u64)address + offset) - ((u64)address % offset)
#define PAGE_SIZE 0x1000ULL
#define NUM_LIST 11
#define MAX_PAGE_SIZE (PAGE_SIZE * (1 << (NUM_LIST - 1)))
//System with 4G
//Note: In x86QSP1 AHCI controller doesn't manage 64bit. G-OS can work with 4G only currently.
#define PHY_MEM_SIZE 0x100000000ULL
#define G_PHY_MEM_SIZE (PHY_MEM_SIZE / 0x40000000ULL)
#define MASTER_PML4 0x400000ULL
#define MASTER_PTR 0x401000ULL
#define MASTER_DIR  0x402000ULL
//I consider an extra page directory (+1) to reserve and extra giga for mapping device reserved memory 
//and initial static structures
#define MASTER_PAGES (0x403000ULL + (0x1000ULL * (G_PHY_MEM_SIZE + 1)))
#define STATIC_BUDDY_STRUCT_START_ADDR (MASTER_PAGES + (0x200000ULL * (G_PHY_MEM_SIZE + 1)))
#define MEM_START_ADDR (STATIC_BUDDY_STRUCT_START_ADDR + (0x300000ULL * G_PHY_MEM_SIZE))
#define MEM_END_ADDR PHY_MEM_SIZE
#define ALLOCATED_MEM (MEM_END_ADDR - MEM_START_ADDR)
#define POOL_START_ADDR MEM_START_ADDR
#define POOL_END_ADDR (POOL_START_ADDR + ((ALLOCATED_MEM / 10ULL) - 1))
#define POOL_NUM 16 
#define MEM_TO_POOL_X ((POOL_END_ADDR - POOL_START_ADDR) / POOL_NUM)
#define MEM_TO_POOL ALIGNED_TO_OFFSET((((POOL_END_ADDR - POOL_START_ADDR) / POOL_NUM) - PAGE_SIZE), PAGE_SIZE)
#define BUDDY_START_ADDR ALIGNED_TO_OFFSET((MEM_START_ADDR + (ALLOCATED_MEM / 10)), MAX_PAGE_SIZE)
#define BUDDY_END_ADDR MEM_END_ADDR
#define BUDDY_MEM_SIZE (BUDDY_END_ADDR - BUDDY_START_ADDR)

#endif
