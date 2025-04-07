#include "lib.h"

void panic()
{
	unsigned long params[1];

	SYSCALL(104,params);
}

void check_free_mem()
{
	unsigned long params[1];

	SYSCALL(103,params);
}

void check_free_pid()
{
	unsigned long params[1];

	SYSCALL(103,params);
}

void check_open_conn()
{
	unsigned long params[1];

	SYSCALL(105,params);
}



