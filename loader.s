.global loader                  # making entry point visible to linker
.global MULTIBOOT_INFO
.global MULTIBOOT_MAGIC

.section .multiboot

# setting up the Multiboot header - see GRUB docs for details
.set MODULEALIGN,	1<<0                   # align loaded modules on page boundaries
.set MEMINFO,      	1<<1                   # provide memory map
.set FLAGS,        	MODULEALIGN | MEMINFO  # this is the Multiboot 'flag' field
.set MAGIC,        	0x1BADB002             # 'magic number' lets bootloader find the header
.set CHECKSUM,     	-(MAGIC + FLAGS)       # checksum required

# not used, garbage padding values.E85250D6
.set HEADER_ADDR, 		0
.set LOAD_ADDR, 	    0
.set LOAD_END_ADDR, 	0
.set BSS_END_ADDR, 		0
.set ENTRY_ADDR, 		0

# RGB: linear graphics mode?
.set MODE_TYPE,			0
# no preference
.set WIDTH,				0
.set HEIGHT,			0
.set DEPTH,				0

.align 4
MultiBootHeader:
.long MAGIC
.long FLAGS
.long CHECKSUM
.long HEADER_ADDR
.long LOAD_ADDR
.long LOAD_END_ADDR
.long BSS_END_ADDR
.long ENTRY_ADDR
.long MODE_TYPE
.long WIDTH
.long HEIGHT
.long DEPTH

.section .bss
.set INIT_STACK_SIZE,0x1000
.set TSS_SIZE, 0x64

.align 0x1000
.lcomm PAGING_DATA,0x4000

.align 0x1000
.lcomm PML4,0x1000
.lcomm PTR,0x1000
.lcomm L_DIR,0x1000
.lcomm L_PAGE,0x8000
.lcomm K_DIR,0x01000
.lcomm K_PAGE,0x8000

.lcomm TSS_ADD,8
.align 32
.lcomm INIT_STACK,INIT_STACK_SIZE   
.align 32
.lcomm TSS,TSS_SIZE

.section .data
.align 4
IDT:
.long 0
.long 0

#Global Descriptor Table
gdt_data:
# null descriptor
.byte 0x00										
.byte 0x00
.byte 0x00
.byte 0x00
.byte 0x00
.byte 0x00
.byte 0x00
.byte 0x00

# gdt kernel code segment
.byte 0x00										
.byte 0x00
.byte 0x00
.byte 0x00
.byte 0x00
.byte 0x9A
.byte 0x20
.byte 0x00

# gdt kernel data segment
.byte 0x00										
.byte 0x00
.byte 0x00
.byte 0x00
.byte 0x00
.byte 0x92
.byte 0x00
.byte 0x00

# gdt user code segment
.byte 0x00										
.byte 0x00
.byte 0x00
.byte 0x00
.byte 0x00
.byte 0xFA
.byte 0x20
.byte 0x00

# gdt user data segment
.byte 0x00										
.byte 0x00
.byte 0x00
.byte 0x00
.byte 0x00
.byte 0xF2
.byte 0x00
.byte 0x00

# gdt tss segment
tss_desc:
.byte TSS_SIZE								
.byte 0x00
.byte 0x00
.byte 0x00
.byte 0x00
.byte 0x89
.byte 0x00
.byte 0x00
.byte 0x00							
.byte 0x00
.byte 0x00
.byte 0x00
.byte 0x00
.byte 0x00
.byte 0x00
.byte 0x00

.align 4
end_of_gdt:
toc: 
.word end_of_gdt - gdt_data - 1 
.long gdt_data

MULTIBOOT_INFO:
.long 0

MULTIBOOT_MAGIC:
.long 0

.section .text
.code32
     
loader:
	cli
	mov %ebx, MULTIBOOT_INFO
	mov %eax, MULTIBOOT_MAGIC
	
	
     
#Build the page map level 4.
    mov $PML4,%ebx
    mov $PTR,%eax
    or $0b11,%eax
    mov %eax,(%ebx)
    
#Build the loader page directory pointer table.
    mov $PTR,%ebx
    mov $L_DIR,%eax
    or $0b11,%eax
    mov %eax,(%ebx)

#Build the loader page directory
	mov $L_DIR,%eax
	mov $L_PAGE,%ecx
	or $0b11,%ecx
	mov $0x9,%ebx
	call fill_page_dir
	
#Build the loader page table
	mov $0x0,%eax
	or $0b11,%eax
	mov $L_PAGE,%ecx
	mov $0x1000,%ebx
	call fill_page
	
#Build the kernel page directory pointer table.
    mov $PTR,%ebx
    mov $K_DIR,%eax
    or $0b11,%eax
    mov %eax,0x80(%ebx)
	
#Build the kernel page directory
	mov $K_DIR,%eax
	mov $K_PAGE,%ecx
	or $0b11,%ecx
	mov $0x11,%ebx
	call fill_page_dir

#Build the loader page table
	mov $0x100000,%eax
	or $0b11,%eax
	mov $K_PAGE, %ecx
	mov $0x1000, %ebx
	call fill_page
 
#Disable IRQs
    mov $0xFF, %al                      		# Out 0xFF to 0xA1 and 0x21 to disable all IRQs.
    out %al, $0xA1
    out %al, $0x21
    nop
    nop
    lidt IDT                        			# Load a zero length IDT so that any NMI causes a triple fault.

#Enter long mode.
    mov $0b10100000, %eax                		# Set the PAE and PGE bit.
    mov %eax, %cr4
    mov $PML4,%ebx                              # Point CR3 at the PML4.
    mov %ebx, %edx                      		
    mov %edx, %cr3
    mov $0xC0000080, %ecx               		# Read from the EFER MSR. 
    rdmsr    
    or $0x00000100, %eax                		# Set the LME bit.
    wrmsr
 
#qui perdo vecchio valore di ebx to check 
    mov %cr0, %ebx                     			# Activate long mode -
    or $0x80000001, %ebx                 		# - by enabling paging and protection simultaneously.
    mov %ebx, %cr0                    
 
    lgdt toc                					# Load GDT
 
    jmp	$0x08,$long_mode             			# Load CS with 64 bit segment and flush the instruction cache
    
fill_page_dir:
	mov $0x1,%edx
.loop_page_dir:
	mov %ecx,(%eax)
	add $0x4,%eax
	mov $0x0,(%eax)
	add $0x4,%eax
	add $0x1000,%ecx
	add $0x1,%edx
	cmp %ebx,%edx
	jb .loop_page_dir
	ret
	
fill_page:
	mov $0x0,%edx
.loop_page_table:
	mov %eax,(%ecx)
   add $0x4,%ecx
	mov $0x0,(%ecx)
   add $0x4, %ecx
   add $0x1000, %eax
   add $0x1,%edx
   cmp %ebx, %edx                		
   jb .loop_page_table
   ret    
         
long_mode:
.code64
	pmode:
	mov	$0x10,%ax	    	
	mov	%ax,%ds
	mov	%ax,%ss
	mov	%ax,%es
	mov $(TSS+0x04),%rax
	mov	%rax,TSS_ADD
	mov $TSS_ADD,%rcx

# Stack grows from botton to up (push decrement address stack,pop increment)
	mov $(INIT_STACK+INIT_STACK_SIZE),%rsp
#Init tss descriptor
	mov 	$TSS,%rax
	mov 	$0xFFFF,%rbx
	and 	%rbx,%rax
	mov 	%ax,tss_desc+2

	mov 	$TSS,%rax
	mov 	$0xFF0000,%rbx
	and 	%rbx,%rax
	shr		$0x10,%rax
	mov 	%al,tss_desc+4

	mov 	$TSS,%rax
	mov 	$0xFF000000,%rbx
	and 	%rbx,%rax
	shr		$0x18,%eax
	mov 	%al,tss_desc+7
	
	mov 	$TSS,%rax
	mov 	$0xFFFFFFFF00000000,%rbx
	and 	%rbx,%rax
	shr		$0x20,%eax
	mov 	%al,tss_desc+8

#load tss       
	mov $0x28,%rax
    ltr %rax
#moltiboot data
	mov MULTIBOOT_INFO, %rax
	mov MULTIBOOT_MAGIC, %rbx
    push 	%rcx
	push  	%rbx                   	
   	push  	%rax                    	
	movabs $kmain, %rdx
	call %rdx            		
	cli
    hlt
