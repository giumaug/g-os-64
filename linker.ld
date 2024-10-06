ENTRY (loader)
kernel_load_address = 0x100000;                              

MEMORY { loader_address_space : ORIGIN = 0x300000, LENGTH = 0x100000 }
MEMORY { kernel_address_space : ORIGIN = 0x400000000, LENGTH = 0x200000 }

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
	 loader.o(.multiboot)
	 *(EXCLUDE_FILE (loader.o).text)
    }>kernel_address_space

    .rodata :AT(kernel_load_address + (rodata - text)){
	rodata = .;
	*(EXCLUDE_FILE (loader.o).rodata)
    }>kernel_address_space 

    .data :AT(kernel_load_address +(data - text)){
	data = .;
	*(EXCLUDE_FILE (loader.o) .data)
    }>kernel_address_space 

    .bss :AT(kernel_load_address + (bss - text)){
	bss = .;
	*(EXCLUDE_FILE (loader.o) .bss)
    }>kernel_address_space
}
