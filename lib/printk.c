#include "asm.h"
#include "lib/lib.h"

static void itoa(unsigned int val,char *char_val,unsigned int base)
{
	unsigned int mod;
	unsigned int res;
	unsigned index=-1;
	unsigned int i;
	char digit;
	char _char_val[11]; 
	if (val<0) *char_val='-';
	res=val;	
	do
	{
		val=res;
		mod=val % 10; 
		res=val/10;
		if (res==0) digit=48+val;
		else digit=48+mod;
		_char_val[++index]=digit;
	}
	while(res!=0);
        for (i=0;i<=index;i++) char_val[i]=_char_val[index-i];
	char_val[++index]='\0';
	return;
}

void static printk_num(int val)
{
	char char_val[11]; //int32
	itoa (val,char_val,10);
	printk(char_val);
}

void static printk_char(char* text)
{
	int index=-1;
	struct t_process_context *current_process_context=system.process_info->current_process[GET_CPU_INDEX]->val;
	t_console_desc *console_desc=current_process_context->console_desc;
	
	while (text[++index]!='\0')
	{
		_write_char_no_irq(console_desc,text[index]);
	}
}

//We accept variadic function of the form f(char*,...) with max 5 variadic arguments of type char*
void printk(char *text,...)
{
	int index = -1;
	int param_index = -1;
	//long long **param_val;
	int params[2];
	struct t_process_context *current_process_context=system.process_info->current_process[GET_CPU_INDEX]->val;
	t_console_desc *console_desc=current_process_context->console_desc;
	void* var_params[6];
	
	GET_FROM_STACK_1(var_params[0]);
	GET_FROM_STACK_2(var_params[1]);
	GET_FROM_STACK_3(var_params[2]);
	GET_FROM_STACK_4(var_params[3]);
	GET_FROM_STACK_5(var_params[4]);
	
	while (text[++index]!='\0')
	{
		if (text[index]=='%' && text[index+1]=='d')
		{
			index+=2;
			param_index++;
			if (param_index < 6)
		    {
				printk_num(var_params[param_index]);
			}
		}
		if (text[index]=='%' && text[index+1]=='s')
		{
			index+=2;
			param_index++;
			if (param_index < 6)
			{
				printk_char((char*) var_params[param_index]);
			}
		}
		_write_char_no_irq(console_desc,text[index]);
	}
}
