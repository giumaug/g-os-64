ENTRY (loader)
kernel_load_address = 0x100000;                              

MEMORY { loader_address_space : ORIGIN = 0x300000, LENGTH = 0x100000 }
MEMORY { kernel_address_space : ORIGIN = 0x400000000, LENGTH = 0x200000 }
MEMORY { ap_trampoline_address_space : ORIGIN = 0x8000, LENGTH = 0x1000 }

SECTIONS{

    /*loader_address_space*/

    .text_1 :{
		loader.o(.text)
		*(EXCLUDE_FILE (loader.o) .code_trampolinerr)
    }>loader_address_space

    .rodata_1 :{
        loader.o(.rodata)
    }>loader_address_space

    .data_1 :{
        loader.o(.data)
        *(EXCLUDE_FILE (loader.o) .code_trampoline)
    }>loader_address_space

    .bss_1 :{
        loader.o(.bss)
    }>loader_address_space

    /* kernel address space */
    
    .pippo_1 :AT(kernel_load_address){
	text_a = .;
	loader.o(.multiboot)
    }>kernel_address_space

    .pippo :AT(kernel_load_address - text_a){
	/*text_a = .;*/
	 /*loader.o(.multiboot)*/
	 *(EXCLUDE_FILE (loader.o) .text)
    }>kernel_address_space

    .rodata :AT(kernel_load_address + (rodata - text_a)){
	rodata = .;
	*(EXCLUDE_FILE (loader.o).rodata)
    }>kernel_address_space 

    .data :AT(kernel_load_address +(data - text_a)){
	data = .;
	*(EXCLUDE_FILE (loader.o) .data)
    }>kernel_address_space 

    .bss :AT(kernel_load_address + (bss - text_a)){
	bss = .;
	*(EXCLUDE_FILE (loader.o) .bss)
    }>kernel_address_space
   
/* 
    .text_2 :{
     test.o(.code_trampoline)
    }>ap_trampoline_address_space

    .data_2 :{
        test.o(.data_trampoline)
    }>ap_trampoline_address_space
*/
}
