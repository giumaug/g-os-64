# 0 "loader.S"
# 0 "<built-in>"
# 0 "<command-line>"
# 1 "/usr/include/stdc-predef.h" 1 3 4
# 0 "<command-line>" 2
# 1 "loader.S"
# 1 "include/general.h" 1
# 2 "loader.S" 2

#Global entries
.global loader
.global ap_trampoline_start
.global ap_trampoline_end
.global MULTIBOOT_INFO
.global MULTIBOOT_MAGIC
.global TSS_ADD
.global AP_GDT
.global AP_TSS
.global AP_GDT_DESC

.section .multiboot

# setting up the Multiboot header - see GRUB docs for details
.set MODULEALIGN, 1<<0 # align loaded modules on page boundaries
.set MEMINFO, 1<<1 # provide memory map
.set FLAGS, MODULEALIGN | MEMINFO # this is the Multiboot 'flag' field
.set MAGIC, 0x1BADB002 # 'magic number' lets bootloader find the header
.set CHECKSUM, -(MAGIC + FLAGS) # checksum required

# not used, garbage padding values.E85250D6
.set HEADER_ADDR, 0
.set LOAD_ADDR, 0
.set LOAD_END_ADDR, 0
.set BSS_END_ADDR, 0
.set ENTRY_ADDR, 0

# RGB: linear graphics mode?
.set MODE_TYPE, 0
# no preference
.set WIDTH, 0
.set HEIGHT, 0
.set DEPTH, 0

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
.lcomm L_PAGE,0x13000
.lcomm K_DIR,0x1000
.lcomm K_PAGE,0x1000

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

#AP PROCESSOR GDT AREA (max 32 cpu)
.align 4
.lcomm AP_GDT, 0x700
#AP PROCESSOR TSS AREA (max 32 cpu)
.align 32
.lcomm AP_TSS, 0x800
#AP GDT ADDRESS
.align 4
.lcomm AP_GDT_DESC, 0x240 #16x64x32
.lcomm AP_GDT_DESC_INDX 4
.lcomm MULTIBOOT_INFO,8
.lcomm MULTIBOOT_MAGIC,8

.section .text
.code32

loader:
    mov $0xFF, 0x400
 cli
 mov %ebx, MULTIBOOT_INFO
 mov %eax, MULTIBOOT_MAGIC

 #mov $kmain, %ecx
 #call %ecx

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
 mov $0x13,%ebx
 call fill_page_dir

#Build the loader page table
 mov $0x0,%eax
 or $0b11,%eax
 mov $L_PAGE,%ecx
 mov $0x2600,%ebx
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
 mov $0x1,%ebx
 call fill_page_dir

#Build the kernel page table
 mov $0x100000,%eax
 or $0b11,%eax
 mov $K_PAGE, %ecx
 mov $0x200, %ebx
 call fill_page

#Disable IRQs
    mov $0xFF, %al # Out 0xFF to 0xA1 and 0x21 to disable all IRQs.
    out %al, $0xA1
    out %al, $0x21
    nop
    nop
    lidt IDT # Load a zero length IDT so that any NMI causes a triple fault.

#Enter long mode.
    mov $0b10100000, %eax # Set the PAE and PGE bit.
    mov %eax, %cr4
    mov $PML4,%ebx # Point CR3 at the PML4.
    mov %ebx, %edx
    mov %edx, %cr3
    mov $0xC0000080, %ecx # Read from the EFER MSR.
    rdmsr
    or $0x00000100, %eax # Set the LME bit.
    wrmsr

#qui perdo vecchio valore di ebx to check
    mov %cr0, %ebx # Activate long mode -
    or $0x80000001, %ebx # - by enabling paging and protection simultaneously.
    mov %ebx, %cr0
    lgdt toc # Load GDT
    jmp $0x08,$long_mode # Load CS with 64 bit segment and flush the instruction cache

fill_page_dir:
 mov $0x0,%edx
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
 mov $0x10,%ax
 mov %ax,%ss
 mov $(TSS+0x04),%rax
 mov %rax,TSS_ADD
# mov $TSS_ADD,%rdx

# Stack grows from botton to up (push decrement address stack,pop increment)
 mov $(INIT_STACK+INIT_STACK_SIZE),%rsp
#Init tss descriptor
 mov $TSS,%rax
 mov $0xFFFF,%rbx
 and %rbx,%rax
 mov %ax,tss_desc+2

 mov $TSS,%rax
 mov $0xFF0000,%rbx
 and %rbx,%rax
 shr $0x10,%rax
 mov %al,tss_desc+4

 mov $TSS,%rax
 mov $0xFF000000,%rbx
 and %rbx,%rax
 shr $0x18,%eax
 mov %al,tss_desc+7

 mov $TSS,%rax
 mov $0xFFFFFFFF00000000,%rbx
 and %rbx,%rax
 shr $0x20,%eax
 mov %al,tss_desc+8

#load tss
 mov $0x28,%rax
    ltr %rax
#moltiboot data
 mov MULTIBOOT_INFO, %rdi
 mov MULTIBOOT_MAGIC, %rsi
 push %rcx
# push %rdx
 push %rsi
    push %rdi
 movabs $kmain, %rcx
 call *%rcx
 cli
    hlt

.section .data_trampoline_16
.align 4
gdt_data_ap:
 .long 0x0 # null descriptor
 .long 0x0

# gdt kernel code segment: # code descriptor
 .word 0xFFFF # limit low
 .word 0x0 # base low
 .byte 0x0 # base middle
 .byte 0B10011010 # access
 .byte 0B11001111 # granularity + high limit
 .byte 0x0 # base high

# gdt kernel data segment: # data descriptor
 .word 0x0FFFF # limit low (Same as code)
 .word 0x0 # base low
 .byte 0x0 # base middle
 .byte 0B10010010 # access
 .byte 0B11001111 # granularity
 .byte 0x0 # base high

end_of_gdt_ap:
toc_ap:
 .word end_of_gdt_ap - gdt_data_ap - 1
 .long gdt_data_ap

.section .code_trampoline_16
.code16
ap_trampoline_start:
    cli
 lgdt toc_ap
 mov %cr0,%eax
 or $0x0001,%eax
 mov %eax,%cr0
 jmp $0x08,$ap_trampoline_pmode

.section .code_trampoline
.code32
ap_trampoline_pmode:
    mov $0b10100000, %eax
    mov %eax, %cr4
    mov $PML4,%ebx
    mov %ebx, %edx
    mov %edx, %cr3
    mov $0xC0000080, %ecx
    rdmsr
    or $0x00000100, %eax
    wrmsr
    mov %cr0, %ebx
    or $0x80000001, %ebx
    mov %ebx, %cr0

##qui perdo vecchio valore di ebx to check
    mov %cr0, %ebx
    or $0x80000001, %ebx
    mov %ebx, %cr0
    lgdt toc
    jmp $0x08,$ap_trampoline_lmode

#The most common and basic method is to execute the CPUID instruction with EAX = 1.
#The APIC ID is returned in the EBX register, specifically in bits 31 to 24 (the upper 8 bits of the

ap_trampoline_lmode:
    mov $0x1, %eax
    #cpuid
    and %ebx, 0xff000000
case_cpu_0:
    cmp %ebx, 0
    jne case_cpu_1
    mov $0, %ax
 #mov %ax, AP_GDT_DESC_INDX
 jmp end_switch
case_cpu_1:
    cmp %ebx, 1
    jne case_cpu_2
    mov $1, %ax
 #mov %ax, AP_GDT_DESC_INDX
 jmp end_switch
case_cpu_2:
    cmp %ebx, 2
    jne case_cpu_3
    mov $2, %ax
 #mov $2, AP_GDT_DESC_INDX
 jmp end_switch
case_cpu_3:
    cmp %ebx, 3
    jne case_cpu_4
    mov $3, %ax
 #mov $3, AP_GDT_DESC_INDX
 jmp end_switch
case_cpu_4:
    cmp %ebx, 4
    jne case_cpu_5
    mov $4, %ax
 #mov $4, AP_GDT_DESC_INDX
 jmp end_switch
case_cpu_5:
    cmp %ebx, 5
    jne case_cpu_6
    mov $5, %ax
 #mov $5, AP_GDT_DESC_INDX
 jmp end_switch
case_cpu_6:
    cmp %ebx, 6
    jne case_cpu_7
    mov $6, %ax
 #mov $6, AP_GDT_DESC_INDX
 jmp end_switch
case_cpu_7:
    cmp %ebx, 7
    jne case_cpu_8
    mov $7, %ax
 #mov $7, AP_GDT_DESC_INDX
 jmp end_switch
case_cpu_8:
    cmp %ebx, 8
    jne case_cpu_9
    mov $8, %ax
 #mov $8, AP_GDT_DESC_INDX
 jmp end_switch
case_cpu_9:
    cmp %ebx, 9
    jne case_cpu_10
    mov $9, %ax
 #mov $9, AP_GDT_DESC_INDX
 jmp end_switch
case_cpu_10:
    cmp %ebx, 10
    jne case_cpu_11
    mov $10, %ax
 #mov $10, AP_GDT_DESC_INDX
 jmp end_switch
case_cpu_11:
    cmp %ebx, 11
    jne case_cpu_12
    mov $11, %ax
 #mov $11, AP_GDT_DESC_INDX
 jmp end_switch
case_cpu_12:
    cmp %ebx, 12
    jne case_cpu_13
    mov $12, %ax
 #mov $12, AP_GDT_DESC_INDX
 jmp end_switch
case_cpu_13:
    cmp %ebx, 13
    jne case_cpu_14
    mov $13, %ax
 #mov $13, AP_GDT_DESC_INDX
 jmp end_switch
case_cpu_14:
    cmp %ebx, 14
    jne case_cpu_15
    mov $14, %ax
 #mov $14, AP_GDT_DESC_INDX
 jmp end_switch
case_cpu_15:
    cmp %ebx, 15
    jne case_cpu_16
    mov $15, %ax
 #mov $15, AP_GDT_DESC_INDX
 jmp end_switch
case_cpu_16:
    cmp %ebx, 16
    jne case_cpu_17
    mov $16, %ax
 #mov $16, AP_GDT_DESC_INDX
 jmp end_switch
case_cpu_17:
    cmp %ebx, 17
    jne case_cpu_18
    mov $17, %ax
 #mov $17, AP_GDT_DESC_INDX
 jmp end_switch
case_cpu_18:
    cmp %ebx, 18
    jne case_cpu_19
    mov $18, %ax
 #mov $18, AP_GDT_DESC_INDX
 jmp end_switch
case_cpu_19:
    cmp %ebx, 19
    jne case_cpu_20
    mov $19, %ax
 #mov $19, AP_GDT_DESC_INDX
 jmp end_switch
case_cpu_20:
    cmp %ebx, 20
    jne case_cpu_21
    mov $20, %ax
 #mov $20, AP_GDT_DESC_INDX
 jmp end_switch
case_cpu_21:
    cmp %ebx, 21
    jne case_cpu_22
    mov $21, %ax
 #mov $21, AP_GDT_DESC_INDX
 jmp end_switch
case_cpu_22:
    cmp %ebx, 22
    jne case_cpu_23
    mov $22, %ax
 #mov $22, AP_GDT_DESC_INDX
 jmp end_switch
case_cpu_23:
    cmp %ebx, 23
    jne case_cpu_24
    mov $23, %ax
 #mov $23, AP_GDT_DESC_INDX
 jmp end_switch
case_cpu_24:
    cmp %ebx, 24
    jne case_cpu_25
    mov $24, %ax
 #mov $24, AP_GDT_DESC_INDX
 jmp end_switch
case_cpu_25:
    cmp %ebx, 25
    jne case_cpu_26
    mov $25, %ax
 #mov $25, AP_GDT_DESC_INDX
 jmp end_switch
case_cpu_26:
    cmp %ebx, 26
    jne case_cpu_27
    mov $26, %ax
 #mov $26, AP_GDT_DESC_INDX
 jmp end_switch
case_cpu_27:
    cmp %ebx, 27
    jne case_cpu_28
    mov $27, %ax
 #mov $27, AP_GDT_DESC_INDX
 jmp end_switch
case_cpu_28:
    cmp %ebx, 28
    jne case_cpu_29
    mov $28, %ax
 #mov $28, AP_GDT_DESC_INDX
 jmp end_switch
case_cpu_29:
    cmp %ebx, 29
    jne case_cpu_30
    mov $29, %ax
 #mov $29, AP_GDT_DESC_INDX
 jmp end_switch
case_cpu_30:
    cmp %ebx, 30
    jne case_cpu_31
    mov $30, %ax
 #mov $30, AP_GDT_DESC_INDX
 jmp end_switch
case_cpu_31:
    cmp %ebx, 31
    mov $31, %ax
 #mov $31, AP_GDT_DESC_INDX

end_switch:
    mov %ax, AP_GDT_DESC_INDX
    mov AP_GDT_DESC, %ecx
    mov AP_GDT_DESC_INDX, %ebx
    imul $6, %ebx
    add %ebx, %ecx
    mov %ecx, AP_GDT_DESC_INDX
    lgdt [AP_GDT_DESC_INDX]
    jmp $0x08,$end_init_ap

end_init_ap:
 hlt
ap_trampoline_end:
 hlt


#TEST
#.code16
#ap_trampoline_start:
# hlt
#ap_trampoline_end:
# hlt




##https:
##https:
