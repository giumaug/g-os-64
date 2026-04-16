#ifndef SPIN_LOCK_H                
#define SPIN_LOCK_H

#include "system.h"

//HINT:%= expands in unique integer

#define ENABLE_PREEMPTION(cpuIndex) asm("lock;decl %0;":"=m"(system.int_path_count[cpuIndex]):"m"(system.int_path_count[cpuIndex]):"memory");
#define DISABLE_PREEMPTION(cpuIndex) asm("lock;incl %0;":"=m"(system.int_path_count[cpuIndex]):"m"(system.int_path_count[cpuIndex]):"memory");
    
#define _SPINLOCK_LOCK(lock)                 \
asm ("                                       \
	_spin%=:		                         \
     	mov $0x1,%%eax;                      \
	xchg %%eax,%0;		                     \
	cmp $0,%%eax;		                     \
	jne _spin%=;		                     \
     "                                       \
    :"+m"((lock).status)::"%eax","memory");
			
#define _SPINLOCK_UNLOCK(lock) 	             \
asm volatile("				                 \
	mov $0,%0		                         \
    "				                         \
    :"+r"((lock).status) :: "memory");

#define SPINLOCK_LOCK(lock,cpuIndex)	     \
	DISABLE_PREEMPTION(cpuIndex)	         \
	_SPINLOCK_LOCK(lock)

#define SPINLOCK_UNLOCK(lock,cpuIndex)	     \
	_SPINLOCK_UNLOCK(lock)	                 \
	ENABLE_PREEMPTION(cpuIndex)		

#define SPINLOCK_INIT(lock) _SPINLOCK_UNLOCK(lock)


typedef struct s_spinlock_desc
{
	unsigned int status;
}	
t_spinlock_desc;

#endif

