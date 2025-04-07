#include "lib.h"

int open(const char *fullpath, int flags)
{
	unsigned long params[3];

	params[0]=fullpath;
	params[1]=flags;
	SYSCALL(18,params);
	return  params[2];
}

void flush_inode_cache()
{
	unsigned long params[1];

	SYSCALL(200, params);
}

