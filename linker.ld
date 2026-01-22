ENTRY (loader)
kernel_load_address = 0x100000;
trampoline_init_address = 0x320000;                        

MEMORY { loader_address_space : ORIGIN = 0x300000, LENGTH = 0x20000 }
MEMORY { kernel_address_space : ORIGIN = 0x400000000, LENGTH = 0x200000 }
MEMORY { ap_trampoline_address_space : ORIGIN = 0x8000, LENGTH = 0x2000 }

SECTIONS{

    /*loader_address_space*/

    .text_1 :{
		loader.o(.text)
    }>loader_address_space

    .rodata_1 :{
        loader.o(.rodata)
    }>loader_address_space

    .data_1 :{
        loader.o(.data)
    }>loader_address_space

    .bss_1 :{
        loader.o(.bss)
    }>loader_address_space

    /* kernel address space */
    
    .text :AT(kernel_load_address){
	text = .;
	KEEP(*(.multiboot)) 
	*(EXCLUDE_FILE (loader.o trampoline.o) .text) 
    }>kernel_address_space

    .rodata :AT(kernel_load_address + (rodata - text)){
	rodata = .;
	*(EXCLUDE_FILE (loader.o trampoline.o) .rodata)
    }>kernel_address_space 

    .data :AT(kernel_load_address +(data - text)){
	data = .;
	*(EXCLUDE_FILE (loader.o trampoline.o) .data)
    }>kernel_address_space 

    .bss :AT(kernel_load_address + (bss - text)){
	bss = .;
	*(EXCLUDE_FILE (loader.o trampoline.o) .bss)
    }>kernel_address_space
   
    .text_2 :AT(trampoline_init_address){
    text_2 = .;
    trampoline.o(.text)
    }>ap_trampoline_address_space
   
    .data_2 :AT(trampoline_init_address + (data_2 - text_2)){
    data_2 = .;
    trampoline.o(.data)
    }>ap_trampoline_address_space
      
    .bss_2 :AT(trampoline_init_address + (bss_2 - text_2)){
    bss_2 = .;
    trampoline.o(.bss)
    }>ap_trampoline_address_space
}
