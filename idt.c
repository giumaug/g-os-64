#include "asm.h" 
#include "idt.h"
#include "scheduler/process.h"
#include "virtual_memory/vm.h"
#include "drivers/pic/8259A.h"
#include "lib/lib.h"
#include "drivers/lapic/lapic.h"

struct t_i_desc idt[255];
struct t_idt_ptr idt_ptr;

void exception()
{
	return;
}

void int_handler_generic() 
{
	struct t_processor_reg processor_reg;
	struct t_processor_reg _processor_reg;
	
	SAVE_PROCESSOR_REG
	//SWITCH_DS_TO_KERNEL_MODE
	exception();
	_processor_reg = processor_reg;
	EOI_TO_LAPIC
	RESTORE_PROCESSOR_REG
	 asm ("movq %0,%%rax;"::"r"(_processor_reg.rax));                                                                                   
	RET_FROM_INT_HANDLER    
}

void int_handler_generic_0() 
{
	struct t_processor_reg processor_reg;
	struct t_processor_reg _processor_reg;
	
	SAVE_PROCESSOR_REG
	//SWITCH_DS_TO_KERNEL_MODE
	exception();
	_processor_reg = processor_reg;
	EOI_TO_LAPIC
	RESTORE_PROCESSOR_REG                                                                                   
	RET_FROM_INT_HANDLER    
}


void int_handler_generic_1() 
{
	struct t_processor_reg processor_reg;
	struct t_processor_reg _processor_reg;
	
	SAVE_PROCESSOR_REG
	//SWITCH_DS_TO_KERNEL_MODE
	exception();
	_processor_reg = processor_reg;
	EOI_TO_LAPIC
	RESTORE_PROCESSOR_REG                                                                                   
	RET_FROM_INT_HANDLER    
}

void int_handler_generic_2() 
{
	struct t_processor_reg processor_reg;
	struct t_processor_reg _processor_reg;
	
	SAVE_PROCESSOR_REG
	//SWITCH_DS_TO_KERNEL_MODE
	exception();
	_processor_reg = processor_reg;
	EOI_TO_LAPIC
	RESTORE_PROCESSOR_REG                                                                                   
	RET_FROM_INT_HANDLER    
}

void int_handler_generic_3() 
{ 
	struct t_processor_reg processor_reg;
	struct t_processor_reg _processor_reg;
	
	SAVE_PROCESSOR_REG
	//SWITCH_DS_TO_KERNEL_MODE
	exception();
	_processor_reg = processor_reg;
	EOI_TO_LAPIC
	RESTORE_PROCESSOR_REG                                                                                   
	RET_FROM_INT_HANDLER    
}

void int_handler_generic_4() 
{
	struct t_processor_reg processor_reg;
	struct t_processor_reg _processor_reg;
	
	SAVE_PROCESSOR_REG
	//SWITCH_DS_TO_KERNEL_MODE
	exception();
	_processor_reg = processor_reg;
	EOI_TO_LAPIC
	RESTORE_PROCESSOR_REG                                                                                   
	RET_FROM_INT_HANDLER    
}

void int_handler_generic_5() 
{
	struct t_processor_reg processor_reg;
	struct t_processor_reg _processor_reg;
	
	SAVE_PROCESSOR_REG
	//SWITCH_DS_TO_KERNEL_MODE
	exception();
	_processor_reg = processor_reg;
	EOI_TO_LAPIC
	RESTORE_PROCESSOR_REG                                                                                   
	RET_FROM_INT_HANDLER    
}

void int_handler_generic_6() 
{
	struct t_processor_reg processor_reg;
	struct t_processor_reg _processor_reg;
	
	SAVE_PROCESSOR_REG
	//SWITCH_DS_TO_KERNEL_MODE
	exception();
	_processor_reg = processor_reg;
	EOI_TO_LAPIC
	RESTORE_PROCESSOR_REG                                                                                   
	RET_FROM_INT_HANDLER    
}

void int_handler_generic_7() 
{
	struct t_processor_reg processor_reg;
	struct t_processor_reg _processor_reg;
	
	SAVE_PROCESSOR_REG
	//SWITCH_DS_TO_KERNEL_MODE
	exception();
	_processor_reg = processor_reg;
	EOI_TO_LAPIC
	RESTORE_PROCESSOR_REG                                                                                   
	RET_FROM_INT_HANDLER    
}

void int_handler_generic_8() 
{
	struct t_processor_reg processor_reg;
	struct t_processor_reg _processor_reg;
	
	SAVE_PROCESSOR_REG
	//SWITCH_DS_TO_KERNEL_MODE
	exception();
	_processor_reg = processor_reg;
	EOI_TO_LAPIC
	RESTORE_PROCESSOR_REG                                                                                   
	RET_FROM_INT_HANDLER    
}

void int_handler_generic_9() 
{
	struct t_processor_reg processor_reg;
	struct t_processor_reg _processor_reg;
	
	SAVE_PROCESSOR_REG
	//SWITCH_DS_TO_KERNEL_MODE
	exception();
	_processor_reg = processor_reg;
	EOI_TO_LAPIC
	RESTORE_PROCESSOR_REG                                                                                   
	RET_FROM_INT_HANDLER    
}

void int_handler_generic_10() 
{
	struct t_processor_reg processor_reg;
	struct t_processor_reg _processor_reg;
	
	SAVE_PROCESSOR_REG
	//SWITCH_DS_TO_KERNEL_MODE
	exception();
	_processor_reg = processor_reg;
	EOI_TO_LAPIC
	RESTORE_PROCESSOR_REG                                                                                   
	RET_FROM_INT_HANDLER    
}

void int_handler_generic_11() 
{
	struct t_processor_reg processor_reg;
	struct t_processor_reg _processor_reg;
	
	SAVE_PROCESSOR_REG
	//SWITCH_DS_TO_KERNEL_MODE
	exception();
	_processor_reg = processor_reg;
	EOI_TO_LAPIC
	RESTORE_PROCESSOR_REG                                                                                   
	RET_FROM_INT_HANDLER    
}

void int_handler_generic_12() 
{
	struct t_processor_reg processor_reg;
	struct t_processor_reg _processor_reg;
	
	SAVE_PROCESSOR_REG
	//SWITCH_DS_TO_KERNEL_MODE
	exception();
	_processor_reg = processor_reg;
	EOI_TO_LAPIC
	RESTORE_PROCESSOR_REG                                                                                   
	RET_FROM_INT_HANDLER    
}

void int_handler_generic_13() 
{
	struct t_processor_reg processor_reg;
	struct t_processor_reg _processor_reg;
	
	SAVE_PROCESSOR_REG
	//SWITCH_DS_TO_KERNEL_MODE
	exception();
	_processor_reg = processor_reg;
	EOI_TO_LAPIC
	RESTORE_PROCESSOR_REG                                                                                   
	RET_FROM_INT_HANDLER    
}

void int_handler_generic_14() 
{
	struct t_processor_reg processor_reg;
	struct t_processor_reg _processor_reg;
	
	SAVE_PROCESSOR_REG
	//SWITCH_DS_TO_KERNEL_MODE
	exception();
	_processor_reg = processor_reg;
	EOI_TO_LAPIC
	RESTORE_PROCESSOR_REG                                                                                   
	RET_FROM_INT_HANDLER    
}

void int_handler_generic_15() 
{
	struct t_processor_reg processor_reg;
	struct t_processor_reg _processor_reg;
	
	SAVE_PROCESSOR_REG
	//SWITCH_DS_TO_KERNEL_MODE
	exception();
	_processor_reg = processor_reg;
	EOI_TO_LAPIC
	RESTORE_PROCESSOR_REG                                                                                   
	RET_FROM_INT_HANDLER    
}

void int_handler_generic_16() 
{
	struct t_processor_reg processor_reg;
	struct t_processor_reg _processor_reg;
	
	SAVE_PROCESSOR_REG
	//SWITCH_DS_TO_KERNEL_MODE
	exception();
	_processor_reg = processor_reg;
	EOI_TO_LAPIC
	RESTORE_PROCESSOR_REG                                                                                   
	RET_FROM_INT_HANDLER    
}

void int_handler_generic_17() 
{
	struct t_processor_reg processor_reg;
	struct t_processor_reg _processor_reg;
	
	SAVE_PROCESSOR_REG
	//SWITCH_DS_TO_KERNEL_MODE
	exception();
	_processor_reg = processor_reg;
	EOI_TO_LAPIC
	RESTORE_PROCESSOR_REG                                                                                   
	RET_FROM_INT_HANDLER    
}

void int_handler_generic_18() 
{
	struct t_processor_reg processor_reg;
	struct t_processor_reg _processor_reg;
	
	SAVE_PROCESSOR_REG
	//SWITCH_DS_TO_KERNEL_MODE
	exception();
	_processor_reg = processor_reg;
	EOI_TO_LAPIC
	RESTORE_PROCESSOR_REG                                                                                   
	RET_FROM_INT_HANDLER    
}

void int_handler_generic_19() 
{
	struct t_processor_reg processor_reg;
	struct t_processor_reg _processor_reg;
	
	SAVE_PROCESSOR_REG
	//SWITCH_DS_TO_KERNEL_MODE
	exception();
	_processor_reg = processor_reg;
	EOI_TO_LAPIC
	RESTORE_PROCESSOR_REG                                                                                   
	RET_FROM_INT_HANDLER    
}

void int_handler_generic_20() 
{
	struct t_processor_reg processor_reg;
	struct t_processor_reg _processor_reg;
	
	SAVE_PROCESSOR_REG
	//SWITCH_DS_TO_KERNEL_MODE
	exception();
	_processor_reg = processor_reg;
	EOI_TO_LAPIC
	RESTORE_PROCESSOR_REG                                                                                   
	RET_FROM_INT_HANDLER    
}

void int_handler_generic_21() 
{
	struct t_processor_reg processor_reg;
	struct t_processor_reg _processor_reg;
	
	SAVE_PROCESSOR_REG
	//SWITCH_DS_TO_KERNEL_MODE
	exception();
	_processor_reg = processor_reg;
	EOI_TO_LAPIC
	RESTORE_PROCESSOR_REG                                                                                   
	RET_FROM_INT_HANDLER    
}

void int_handler_generic_22() 
{
	struct t_processor_reg processor_reg;
	struct t_processor_reg _processor_reg;
	
	SAVE_PROCESSOR_REG
	//SWITCH_DS_TO_KERNEL_MODE
	exception();
	_processor_reg = processor_reg;
	EOI_TO_LAPIC
	RESTORE_PROCESSOR_REG                                                                                   
	RET_FROM_INT_HANDLER    
}

void int_handler_generic_23() 
{
	struct t_processor_reg processor_reg;
	struct t_processor_reg _processor_reg;
	
	SAVE_PROCESSOR_REG
	//SWITCH_DS_TO_KERNEL_MODE
	exception();
	_processor_reg = processor_reg;
	EOI_TO_LAPIC
	RESTORE_PROCESSOR_REG                                                                                   
	RET_FROM_INT_HANDLER    
}

void int_handler_generic_24() 
{
	struct t_processor_reg processor_reg;
	struct t_processor_reg _processor_reg;
	
	SAVE_PROCESSOR_REG
	//SWITCH_DS_TO_KERNEL_MODE
	exception();
	_processor_reg = processor_reg;
	EOI_TO_LAPIC
	RESTORE_PROCESSOR_REG                                                                                   
	RET_FROM_INT_HANDLER    
}

void int_handler_generic_25() 
{
	struct t_processor_reg processor_reg;
	struct t_processor_reg _processor_reg;
	
	SAVE_PROCESSOR_REG
	//SWITCH_DS_TO_KERNEL_MODE
	exception();
	_processor_reg = processor_reg;
	EOI_TO_LAPIC
	RESTORE_PROCESSOR_REG                                                                                   
	RET_FROM_INT_HANDLER    
}

void int_handler_generic_26() 
{
	struct t_processor_reg processor_reg;
	struct t_processor_reg _processor_reg;
	
	SAVE_PROCESSOR_REG
	//SWITCH_DS_TO_KERNEL_MODE
	exception();
	_processor_reg = processor_reg;
	EOI_TO_LAPIC
	RESTORE_PROCESSOR_REG                                                                                   
	RET_FROM_INT_HANDLER    
}

void int_handler_generic_27() 
{
	struct t_processor_reg processor_reg;
	struct t_processor_reg _processor_reg;
	
	SAVE_PROCESSOR_REG
	//SWITCH_DS_TO_KERNEL_MODE
	exception();
	_processor_reg = processor_reg;
	EOI_TO_LAPIC
	RESTORE_PROCESSOR_REG                                                                                   
	RET_FROM_INT_HANDLER    
}

void int_handler_generic_28() 
{
	struct t_processor_reg processor_reg;
	struct t_processor_reg _processor_reg;
	
	SAVE_PROCESSOR_REG
	//SWITCH_DS_TO_KERNEL_MODE
	exception();
	_processor_reg = processor_reg;
	EOI_TO_LAPIC
	RESTORE_PROCESSOR_REG                                                                                   
	RET_FROM_INT_HANDLER    
}

void int_handler_generic_29() 
{
	struct t_processor_reg processor_reg;
	struct t_processor_reg _processor_reg;
	
	SAVE_PROCESSOR_REG
	//SWITCH_DS_TO_KERNEL_MODE
	exception();
	_processor_reg = processor_reg;
	EOI_TO_LAPIC
	RESTORE_PROCESSOR_REG                                                                                   
	RET_FROM_INT_HANDLER    
}

void int_handler_generic_30() 
{
	struct t_processor_reg processor_reg;
	struct t_processor_reg _processor_reg;
	
	SAVE_PROCESSOR_REG
	//SWITCH_DS_TO_KERNEL_MODE
	exception();
	_processor_reg = processor_reg;
	EOI_TO_LAPIC
	RESTORE_PROCESSOR_REG                                                                                   
	RET_FROM_INT_HANDLER    
}

void int_handler_generic_31() 
{
	struct t_processor_reg processor_reg;
	struct t_processor_reg _processor_reg;
	
	SAVE_PROCESSOR_REG
	//SWITCH_DS_TO_KERNEL_MODE
	exception();
	_processor_reg = processor_reg;
	EOI_TO_LAPIC
	RESTORE_PROCESSOR_REG                                                                                   
	RET_FROM_INT_HANDLER    
}

void int_handler_generic_32() 
{
	struct t_processor_reg processor_reg;
	struct t_processor_reg _processor_reg;
	
	SAVE_PROCESSOR_REG
	//SWITCH_DS_TO_KERNEL_MODE
	exception();
	_processor_reg = processor_reg;
	EOI_TO_LAPIC
	RESTORE_PROCESSOR_REG                                                                                   
	RET_FROM_INT_HANDLER    
}

void int_handler_generic_33() 
{
	struct t_processor_reg processor_reg;
	struct t_processor_reg _processor_reg;
	
	SAVE_PROCESSOR_REG
	//SWITCH_DS_TO_KERNEL_MODE
	exception();
	_processor_reg = processor_reg;
	EOI_TO_LAPIC
	RESTORE_PROCESSOR_REG                                                                                   
	RET_FROM_INT_HANDLER    
}

void int_handler_generic_34() 
{
	struct t_processor_reg processor_reg;
	struct t_processor_reg _processor_reg;
	
	SAVE_PROCESSOR_REG
	//SWITCH_DS_TO_KERNEL_MODE
	exception();
	_processor_reg = processor_reg;
	EOI_TO_LAPIC
	RESTORE_PROCESSOR_REG                                                                                   
	RET_FROM_INT_HANDLER    
}

void int_handler_generic_35() 
{
	struct t_processor_reg processor_reg;
	struct t_processor_reg _processor_reg;
	
	SAVE_PROCESSOR_REG
	//SWITCH_DS_TO_KERNEL_MODE
	exception();
	_processor_reg = processor_reg;
	EOI_TO_LAPIC
	RESTORE_PROCESSOR_REG                                                                                   
	RET_FROM_INT_HANDLER    
}

void int_handler_generic_36() 
{
	struct t_processor_reg processor_reg;
	struct t_processor_reg _processor_reg;
	
	SAVE_PROCESSOR_REG
	//SWITCH_DS_TO_KERNEL_MODE
	exception();
	_processor_reg = processor_reg;
	EOI_TO_LAPIC
	RESTORE_PROCESSOR_REG                                                                                   
	RET_FROM_INT_HANDLER    
}

void int_handler_generic_37() 
{
	struct t_processor_reg processor_reg;
	struct t_processor_reg _processor_reg;
	
	SAVE_PROCESSOR_REG
	//SWITCH_DS_TO_KERNEL_MODE
	exception();
	_processor_reg = processor_reg;
	EOI_TO_LAPIC
	RESTORE_PROCESSOR_REG                                                                                   
	RET_FROM_INT_HANDLER    
}

void int_handler_generic_38() 
{
	struct t_processor_reg processor_reg;
	struct t_processor_reg _processor_reg;
	
	SAVE_PROCESSOR_REG
	//SWITCH_DS_TO_KERNEL_MODE
	exception();
	_processor_reg = processor_reg;
	EOI_TO_LAPIC
	RESTORE_PROCESSOR_REG                                                                                   
	RET_FROM_INT_HANDLER    
}

void int_handler_generic_39() 
{
	struct t_processor_reg processor_reg;
	struct t_processor_reg _processor_reg;
	
	SAVE_PROCESSOR_REG
	//SWITCH_DS_TO_KERNEL_MODE
	exception();
	_processor_reg = processor_reg;
	EOI_TO_LAPIC
	RESTORE_PROCESSOR_REG                                                                                   
	RET_FROM_INT_HANDLER    
}

void int_handler_generic_40() 
{
	struct t_processor_reg processor_reg;
	struct t_processor_reg _processor_reg;
	
	SAVE_PROCESSOR_REG
	//SWITCH_DS_TO_KERNEL_MODE
	exception();
	_processor_reg = processor_reg;
	EOI_TO_LAPIC
	RESTORE_PROCESSOR_REG                                                                                   
	RET_FROM_INT_HANDLER    
}

void int_handler_generic_41() 
{
	struct t_processor_reg processor_reg;
	struct t_processor_reg _processor_reg;
	
	SAVE_PROCESSOR_REG
	//SWITCH_DS_TO_KERNEL_MODE
	exception();
	_processor_reg = processor_reg;
	EOI_TO_LAPIC
	RESTORE_PROCESSOR_REG                                                                                   
	RET_FROM_INT_HANDLER    
}

void int_handler_generic_42() 
{
	struct t_processor_reg processor_reg;
	struct t_processor_reg _processor_reg;
	
	SAVE_PROCESSOR_REG
	//SWITCH_DS_TO_KERNEL_MODE
	exception();
	_processor_reg = processor_reg;
	EOI_TO_LAPIC
	RESTORE_PROCESSOR_REG                                                                                   
	RET_FROM_INT_HANDLER    
}

void int_handler_generic_43() 
{
	struct t_processor_reg processor_reg;
	struct t_processor_reg _processor_reg;
	
	SAVE_PROCESSOR_REG
	//SWITCH_DS_TO_KERNEL_MODE
	exception();
	_processor_reg = processor_reg;
	EOI_TO_LAPIC
	RESTORE_PROCESSOR_REG                                                                                   
	RET_FROM_INT_HANDLER    
}

void int_handler_generic_44() 
{
	struct t_processor_reg processor_reg;
	struct t_processor_reg _processor_reg;
	
	SAVE_PROCESSOR_REG
	//SWITCH_DS_TO_KERNEL_MODE
	exception();
	_processor_reg = processor_reg;
	EOI_TO_LAPIC
	RESTORE_PROCESSOR_REG                                                                                   
	RET_FROM_INT_HANDLER    
}

void int_handler_generic_45() 
{
	struct t_processor_reg processor_reg;
	struct t_processor_reg _processor_reg;
	
	SAVE_PROCESSOR_REG
	//SWITCH_DS_TO_KERNEL_MODE
	exception();
	_processor_reg = processor_reg;
	EOI_TO_LAPIC
	RESTORE_PROCESSOR_REG                                                                                   
	RET_FROM_INT_HANDLER    
}

void int_handler_generic_46() 
{
	struct t_processor_reg processor_reg;
	struct t_processor_reg _processor_reg;
	
	SAVE_PROCESSOR_REG
	//SWITCH_DS_TO_KERNEL_MODE
	exception();
	_processor_reg = processor_reg;
	EOI_TO_LAPIC
	RESTORE_PROCESSOR_REG                                                                                   
	RET_FROM_INT_HANDLER    
}

void int_handler_generic_47() 
{
	struct t_processor_reg processor_reg;
	struct t_processor_reg _processor_reg;
	
	SAVE_PROCESSOR_REG
	//SWITCH_DS_TO_KERNEL_MODE
	exception();
	_processor_reg = processor_reg;
	EOI_TO_LAPIC
	RESTORE_PROCESSOR_REG                                                                                   
	RET_FROM_INT_HANDLER    
}

void int_handler_generic_48() 
{
	struct t_processor_reg processor_reg;
	struct t_processor_reg _processor_reg;
	
	SAVE_PROCESSOR_REG
	//SWITCH_DS_TO_KERNEL_MODE
	exception();
	_processor_reg = processor_reg;
	EOI_TO_LAPIC
	RESTORE_PROCESSOR_REG                                                                                   
	RET_FROM_INT_HANDLER    
}

void int_handler_generic_49() 
{
	struct t_processor_reg processor_reg;
	struct t_processor_reg _processor_reg;
	
	SAVE_PROCESSOR_REG
	//SWITCH_DS_TO_KERNEL_MODE
	exception();
	_processor_reg = processor_reg;
	EOI_TO_LAPIC
	RESTORE_PROCESSOR_REG                                                                                   
	RET_FROM_INT_HANDLER    
}

void int_handler_generic_50() 
{
	struct t_processor_reg processor_reg;
	struct t_processor_reg _processor_reg;
	
	SAVE_PROCESSOR_REG
	//SWITCH_DS_TO_KERNEL_MODE
	exception();
	_processor_reg = processor_reg;
	EOI_TO_LAPIC
	RESTORE_PROCESSOR_REG                                                                                   
	RET_FROM_INT_HANDLER    
}

void init_idt()
{
	int i;
	
	for (i=0;i<255;i++)
	{
		idt[i].baseLow=((u16)(&int_handler_generic)) & 0xFFFF;
		idt[i].selector=0x8;
		idt[i].flags=0x08e00;
		idt[i].baseHi=((u16)(&int_handler_generic))>>0x010;
		idt[i].baseExt=((u32)(&int_handler_generic))>>0x020;
		idt[i].pad=0;
	}

/*
	idt[0].baseLow=((u16)(&int_handler_generic_0)) & 0xFFFF;
	idt[0].selector=0x8;
	idt[0].flags=0x08e00;
	idt[0].baseHi=((u16)(&int_handler_generic_0))>>0x010;
	idt[0].baseExt=((u32)(&int_handler_generic_0))>>0x020;
	idt[0].pad=0;

	idt[1].baseLow=((u16)(&int_handler_generic_1)) & 0xFFFF;
	idt[1].selector=0x8;
	idt[1].flags=0x08e00;
	idt[1].baseHi=((u16)(&int_handler_generic_1))>>0x010;
	idt[1].baseExt=((u32)(&int_handler_generic_1))>>0x020;
	idt[1].pad=0;

	idt[2].baseLow=((u16)(&int_handler_generic_2)) & 0xFFFF;
	idt[2].selector=0x8;
	idt[2].flags=0x08e00;
	idt[2].baseHi=((u16)(&int_handler_generic_2))>>0x010;
	idt[2].baseExt=((u32)(&int_handler_generic_2))>>0x020;
	idt[2].pad=0;

	idt[3].baseLow=((u16)(&int_handler_generic_3)) & 0xFFFF;
	idt[3].selector=0x8;
	idt[3].flags=0x08e00;
	idt[3].baseHi=((u16)(&int_handler_generic_3))>>0x010;
	idt[3].baseExt=((u32)(&int_handler_generic_3))>>0x020;
	idt[3].pad=0;

	idt[4].baseLow=((u16)(&int_handler_generic_4)) & 0xFFFF;
	idt[4].selector=0x8;
	idt[4].flags=0x08e00;
	idt[4].baseHi=((u16)(&int_handler_generic_4))>>0x010;
	idt[4].baseExt=((u32)(&int_handler_generic_4))>>0x020;
	idt[4].pad=0;

	idt[5].baseLow=((u16)(&int_handler_generic_5)) & 0xFFFF;
	idt[5].selector=0x8;
	idt[5].flags=0x08e00;
	idt[5].baseHi=((u16)(&int_handler_generic_5))>>0x010;
	idt[5].baseExt=((u32)(&int_handler_generic_5))>>0x020;
	idt[5].pad=0;

	idt[6].baseLow=((u16)(&int_handler_generic_6)) & 0xFFFF;
	idt[6].selector=0x8;
	idt[6].flags=0x08e00;
	idt[6].baseHi=((u16)(&int_handler_generic_6))>>0x010;
	idt[6].baseExt=((u32)(&int_handler_generic_6))>>0x020;
	idt[6].pad=0;

	idt[7].baseLow=((u16)(&int_handler_generic_7)) & 0xFFFF;
	idt[7].selector=0x8;
	idt[7].flags=0x08e00;
	idt[7].baseHi=((u16)(&int_handler_generic_7))>>0x010;
	idt[7].baseExt=((u32)(&int_handler_generic_7))>>0x020;
	idt[7].pad=0;

	idt[8].baseLow=((u16)(&int_handler_generic_8)) & 0xFFFF;
	idt[8].selector=0x8;
	idt[8].flags=0x08e00;
	idt[8].baseHi=((u16)(&int_handler_generic_8))>>0x010;
	idt[8].baseExt=((u32)(&int_handler_generic_8))>>0x020;
	idt[8].pad=0;

	idt[9].baseLow=((u16)(&int_handler_generic_9)) & 0xFFFF;
	idt[9].selector=0x8;
	idt[9].flags=0x08e00;
	idt[9].baseHi=((u16)(&int_handler_generic_9))>>0x010;
	idt[9].baseExt=((u32)(&int_handler_generic_9))>>0x020;
	idt[9].pad=0;

	idt[10].baseLow=((u16)(&int_handler_generic_10)) & 0xFFFF;
	idt[10].selector=0x8;
	idt[10].flags=0x08e00;
	idt[10].baseHi=((u16)(&int_handler_generic_10))>>0x010;
	idt[10].baseExt=((u32)(&int_handler_generic_10))>>0x020;
	idt[10].pad=0;

	idt[11].baseLow=((u16)(&int_handler_generic_11)) & 0xFFFF;
	idt[11].selector=0x8;
	idt[11].flags=0x08e00;
	idt[11].baseHi=((u16)(&int_handler_generic_11))>>0x010;
	idt[11].baseExt=((u32)(&int_handler_generic_11))>>0x020;
	idt[11].pad=0;

	idt[12].baseLow=((u16)(&int_handler_generic_12)) & 0xFFFF;
	idt[12].selector=0x8;
	idt[12].flags=0x08e00;
	idt[12].baseHi=((u16)(&int_handler_generic_12))>>0x010;
	idt[12].baseExt=((u32)(&int_handler_generic_12))>>0x020;
	idt[12].pad=0;

	idt[13].baseLow=((u16)(&int_handler_generic_13)) & 0xFFFF;
	idt[13].selector=0x8;
	idt[13].flags=0x08e00;
	idt[13].baseHi=((u16)(&int_handler_generic_13))>>0x010;
	idt[13].baseExt=((u32)(&int_handler_generic_13))>>0x020;
	idt[13].pad=0;

	idt[14].baseLow=((u16)(&int_handler_generic_14)) & 0xFFFF;
	idt[14].selector=0x8;
	idt[14].flags=0x08e00;
	idt[14].baseHi=((u16)(&int_handler_generic_14))>>0x010;
	idt[14].baseExt=((u32)(&int_handler_generic_14))>>0x020;
	idt[14].pad=0;

	idt[15].baseLow=((u16)(&int_handler_generic_14)) & 0xFFFF;
	idt[15].selector=0x8;
	idt[15].flags=0x08e00;
	idt[15].baseHi=((u16)(&int_handler_generic_15))>>0x010;
	idt[15].baseExt=((u32)(&int_handler_generic_15))>>0x020;
	idt[15].pad=0;

	idt[16].baseLow=((u16)(&int_handler_generic_16)) & 0xFFFF;
	idt[16].selector=0x8;
	idt[16].flags=0x08e00;
	idt[16].baseHi=((u16)(&int_handler_generic_16))>>0x010;
	idt[16].baseExt=((u32)(&int_handler_generic_16))>>0x020;
	idt[16].pad=0;

	idt[17].baseLow=((u16)(&int_handler_generic_17)) & 0xFFFF;
	idt[17].selector=0x8;
	idt[17].flags=0x08e00;
	idt[17].baseHi=((u16)(&int_handler_generic_17))>>0x010;
	idt[17].baseExt=((u32)(&int_handler_generic_17))>>0x020;
	idt[17].pad=0;

	idt[18].baseLow=((u16)(&int_handler_generic_18)) & 0xFFFF;
	idt[18].selector=0x8;
	idt[18].flags=0x08e00;
	idt[18].baseHi=((u16)(&int_handler_generic_18))>>0x010;
	idt[18].baseExt=((u32)(&int_handler_generic_18))>>0x020;
	idt[18].pad=0;

	idt[19].baseLow=((u16)(&int_handler_generic_19)) & 0xFFFF;
	idt[19].selector=0x8;
	idt[19].flags=0x08e00;
	idt[19].baseHi=((u16)(&int_handler_generic_19))>>0x010;
	idt[19].baseExt=((u32)(&int_handler_generic_19))>>0x020;
	idt[19].pad=0;

	idt[20].baseLow=((u16)(&int_handler_generic_20)) & 0xFFFF;
	idt[20].selector=0x8;
	idt[20].flags=0x08e00;
	idt[20].baseHi=((u16)(&int_handler_generic_20))>>0x010;
	idt[20].baseExt=((u32)(&int_handler_generic_20))>>0x020;
	idt[20].pad=0;

	idt[21].baseLow=((u16)(&int_handler_generic_21)) & 0xFFFF;
	idt[21].selector=0x8;
	idt[21].flags=0x08e00;
	idt[21].baseHi=((u16)(&int_handler_generic_21))>>0x010;
	idt[21].baseExt=((u32)(&int_handler_generic_21))>>0x020;
	idt[21].pad=0;

	idt[22].baseLow=((u16)(&int_handler_generic_22)) & 0xFFFF;
	idt[22].selector=0x8;
	idt[22].flags=0x08e00;
	idt[22].baseHi=((u16)(&int_handler_generic_22))>>0x010;
	idt[22].baseExt=((u32)(&int_handler_generic_22))>>0x020;
	idt[22].pad=0;

	idt[23].baseLow=((u16)(&int_handler_generic_23)) & 0xFFFF;
	idt[23].selector=0x8;
	idt[23].flags=0x08e00;
	idt[23].baseHi=((u16)(&int_handler_generic_23))>>0x010;
	idt[23].baseExt=((u32)(&int_handler_generic_23))>>0x020;
	idt[23].pad=0;

	idt[24].baseLow=((u16)(&int_handler_generic_24)) & 0xFFFF;
	idt[24].selector=0x8;
	idt[24].flags=0x08e00;
	idt[24].baseHi=((u16)(&int_handler_generic_24))>>0x010;
	idt[24].baseExt=((u32)(&int_handler_generic_24))>>0x020;
	idt[24].pad=0;

	idt[25].baseLow=((u16)(&int_handler_generic_25)) & 0xFFFF;
	idt[25].selector=0x8;
	idt[25].flags=0x08e00;
	idt[25].baseHi=((u16)(&int_handler_generic_25))>>0x010;
	idt[25].baseExt=((u32)(&int_handler_generic_25))>>0x020;
	idt[25].pad=0;

	idt[26].baseLow=((u16)(&int_handler_generic_26)) & 0xFFFF;
	idt[26].selector=0x8;
	idt[26].flags=0x08e00;
	idt[26].baseHi=((u16)(&int_handler_generic_26))>>0x010;
	idt[26].baseExt=((u32)(&int_handler_generic_26))>>0x020;
	idt[26].pad=0;

	idt[27].baseLow=((u16)(&int_handler_generic_27)) & 0xFFFF;
	idt[27].selector=0x8;
	idt[27].flags=0x08e00;
	idt[27].baseHi=((u16)(&int_handler_generic_27))>>0x010;
	idt[27].baseExt=((u32)(&int_handler_generic_27))>>0x020;
	idt[27].pad=0;

	idt[28].baseLow=((u16)(&int_handler_generic_28)) & 0xFFFF;
	idt[28].selector=0x8;
	idt[28].flags=0x08e00;
	idt[28].baseHi=((u16)(&int_handler_generic_28))>>0x010;
	idt[28].baseExt=((u32)(&int_handler_generic_28))>>0x020;
	idt[28].pad=0;

	idt[29].baseLow=((u16)(&int_handler_generic_29)) & 0xFFFF;
	idt[29].selector=0x8;
	idt[29].flags=0x08e00;
	idt[29].baseHi=((u16)(&int_handler_generic_29))>>0x010;
	idt[29].baseExt=((u32)(&int_handler_generic_29))>>0x020;
	idt[29].pad=0;

	idt[30].baseLow=((u16)(&int_handler_generic_30)) & 0xFFFF;
	idt[30].selector=0x8;
	idt[30].flags=0x08e00;
	idt[30].baseHi=((u16)(&int_handler_generic_30))>>0x010;
	idt[30].baseExt=((u32)(&int_handler_generic_30))>>0x020;
	idt[30].pad=0;

	idt[31].baseLow=((u16)(&int_handler_generic_31)) & 0xFFFF;
	idt[31].selector=0x8;
	idt[31].flags=0x08e00;
	idt[31].baseHi=((u16)(&int_handler_generic_31))>>0x010;
	idt[31].baseExt=((u32)(&int_handler_generic_31))>>0x020;
	idt[31].pad=0;

	idt[32].baseLow=((u16)(&int_handler_generic_32)) & 0xFFFF;
	idt[32].selector=0x8;
	idt[32].flags=0x08e00;
	idt[32].baseHi=((u16)(&int_handler_generic_32))>>0x010;
	idt[32].baseExt=((u32)(&int_handler_generic_32))>>0x020;
	idt[32].pad=0;

	idt[33].baseLow=((u16)(&int_handler_generic_33)) & 0xFFFF;
	idt[33].selector=0x8;
	idt[33].flags=0x08e00;
	idt[33].baseHi=((u16)(&int_handler_generic_33))>>0x010;
	idt[33].baseExt=((u32)(&int_handler_generic_33))>>0x020;
	idt[33].pad=0;

	idt[34].baseLow=((u16)(&int_handler_generic_34)) & 0xFFFF;
	idt[34].selector=0x8;
	idt[34].flags=0x08e00;
	idt[34].baseHi=((u16)(&int_handler_generic_34))>>0x010;
	idt[34].baseExt=((u32)(&int_handler_generic_34))>>0x020;
	idt[34].pad=0;

	idt[35].baseLow=((u16)(&int_handler_generic_35)) & 0xFFFF;
	idt[35].selector=0x8;
	idt[35].flags=0x08e00;
	idt[35].baseHi=((u16)(&int_handler_generic_35))>>0x010;
	idt[35].baseExt=((u32)(&int_handler_generic_35))>>0x020;
	idt[35].pad=0;

	idt[36].baseLow=((u16)(&int_handler_generic_36)) & 0xFFFF;
	idt[36].selector=0x8;
	idt[36].flags=0x08e00;
	idt[36].baseHi=((u16)(&int_handler_generic_36))>>0x010;
	idt[36].baseExt=((u32)(&int_handler_generic_36))>>0x020;
	idt[36].pad=0;

	idt[37].baseLow=((u16)(&int_handler_generic_37)) & 0xFFFF;
	idt[37].selector=0x8;
	idt[37].flags=0x08e00;
	idt[37].baseHi=((u16)(&int_handler_generic_37))>>0x010;
	idt[37].baseExt=((u32)(&int_handler_generic_37))>>0x020;
	idt[37].pad=0;

	idt[38].baseLow=((u16)(&int_handler_generic_38)) & 0xFFFF;
	idt[38].selector=0x8;
	idt[38].flags=0x08e00;
	idt[38].baseHi=((u16)(&int_handler_generic_38))>>0x010;
	idt[38].baseExt=((u32)(&int_handler_generic_38))>>0x020;
	idt[38].pad=0;

	idt[39].baseLow=((u16)(&int_handler_generic_39)) & 0xFFFF;
	idt[39].selector=0x8;
	idt[39].flags=0x08e00;
	idt[39].baseHi=((u16)(&int_handler_generic_39))>>0x010;
	idt[39].baseExt=((u32)(&int_handler_generic_39))>>0x020;
	idt[39].pad=0;

	idt[40].baseLow=((u16)(&int_handler_generic_40)) & 0xFFFF;
	idt[40].selector=0x8;
	idt[40].flags=0x08e00;
	idt[40].baseHi=((u16)(&int_handler_generic_40))>>0x010;
	idt[40].baseExt=((u32)(&int_handler_generic_40))>>0x020;
	idt[40].pad=0;

	idt[41].baseLow=((u16)(&int_handler_generic_41)) & 0xFFFF;
	idt[41].selector=0x8;
	idt[41].flags=0x08e00;
	idt[41].baseHi=((u16)(&int_handler_generic_41))>>0x010;
	idt[41].baseExt=((u32)(&int_handler_generic_41))>>0x020;
	idt[41].pad=0;

	idt[42].baseLow=((u16)(&int_handler_generic_42)) & 0xFFFF;
	idt[42].selector=0x8;
	idt[42].flags=0x08e00;
	idt[42].baseHi=((u16)(&int_handler_generic_42))>>0x010;
	idt[42].baseExt=((u32)(&int_handler_generic_42))>>0x020;
	idt[42].pad=0;

	idt[43].baseLow=((u16)(&int_handler_generic_43)) & 0xFFFF;
	idt[43].selector=0x8;
	idt[43].flags=0x08e00;
	idt[43].baseHi=((u16)(&int_handler_generic_43))>>0x010;
	idt[43].baseExt=((u32)(&int_handler_generic_43))>>0x020;
	idt[43].pad=0;

	idt[44].baseLow=((u16)(&int_handler_generic_44)) & 0xFFFF;
	idt[44].selector=0x8;
	idt[44].flags=0x08e00;
	idt[44].baseHi=((u16)(&int_handler_generic_44))>>0x010;
	idt[44].baseExt=((u32)(&int_handler_generic_44))>>0x020;
	idt[44].pad=0;

	idt[45].baseLow=((u16)(&int_handler_generic_45)) & 0xFFFF;
	idt[45].selector=0x8;
	idt[45].flags=0x08e00;
	idt[45].baseHi=((u16)(&int_handler_generic_45))>>0x010;
	idt[45].baseExt=((u32)(&int_handler_generic_45))>>0x020;
	idt[45].pad=0;

	idt[46].baseLow=((u16)(&int_handler_generic_46)) & 0xFFFF;
	idt[46].selector=0x8;
	idt[46].flags=0x08e00;
	idt[46].baseHi=((u16)(&int_handler_generic_46))>>0x010;
	idt[46].baseExt=((u32)(&int_handler_generic_46))>>0x020;
	idt[46].pad=0;

	idt[47].baseLow=((u16)(&int_handler_generic_47)) & 0xFFFF;
	idt[47].selector=0x8;
	idt[47].flags=0x08e00;
	idt[47].baseHi=((u16)(&int_handler_generic_47))>>0x010;
	idt[47].baseExt=((u32)(&int_handler_generic_47))>>0x020;
	idt[47].pad=0;

	idt[48].baseLow=((u16)(&int_handler_generic_48)) & 0xFFFF;
	idt[48].selector=0x8;
	idt[48].flags=0x08e00;
	idt[48].baseHi=((u16)(&int_handler_generic_48))>>0x010;
	idt[48].baseExt=((u32)(&int_handler_generic_48))>>0x020;
	idt[48].pad=0;

	idt[49].baseLow=((u16)(&int_handler_generic_49)) & 0xFFFF;
	idt[49].selector=0x8;
	idt[49].flags=0x08e00;
	idt[49].baseHi=((u16)(&int_handler_generic_49))>>0x010;
	idt[49].baseExt=((u32)(&int_handler_generic_49))>>0x020;
	idt[49].pad=0;

	idt[50].baseLow=((u16)(&int_handler_generic_50)) & 0xFFFF;
	idt[50].selector=0x8;
	idt[50].flags=0x08e00;
	idt[50].baseHi=((u16)(&int_handler_generic_50))>>0x010;
	idt[50].baseExt=((u32)(&int_handler_generic_50))>>0x020;
	idt[50].pad=0;
*/
	
	idt_ptr.idt_size=128*256;
    idt_ptr.idt_address=(u64)idt;
	asm( "lidt %0" :: "m"(idt_ptr));
}

void set_idt_entry(int entry,struct t_i_desc* i_desc)
{
	idt[entry]=*i_desc;
}

