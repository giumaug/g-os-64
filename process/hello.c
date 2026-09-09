#include "lib/lib.h"

int main()
{
	int i = 0;

	for (i = 0;i < 1;i++) 
	{
		printf("hello!!! \n");
		if(fork() == 0) 
		{
			printf("hello from child \n");
			exit(0);
		}
		else 
		{
			//nothing
		}
	}
	//sleep(5000);
	exit(0);
}
