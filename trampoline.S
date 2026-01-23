#include "include/general.h"
#include "include/memory_manager/general.h"

.global ap_trampoline_start
.global ap_trampoline_end
.global AP_GDT
.global AP_TSS
.global AP_GDT_DESC

.section .bss
.lcomm AP_IDT 8
#AP PROCESSOR GDT AREA (max 32 cpu)
.align 4
.lcomm AP_GDT, 0x700
#AP PROCESSOR TSS AREA (max 32 cpu)
.align 32
.lcomm AP_TSS, 0x880
#AP GDT ADDRESS
.align 4
.lcomm AP_GDT_DESC, 0x200 #10x32 (max 32 cpu) I reserve 16 bytes for each gdt_desc for memory alignment.

.section .data
.align 4
gdt_data_ap: 
	.long 0x0 			# null descriptor
	.long 0x0 
 
# gdt kernel code segment:		# code descriptor
	.word 0xFFFF 				# limit low
	.word 0x0 					# base low
	.byte 0x0 					# base middle
	.byte 0B10011010 			# access
	.byte 0B11001111 			# granularity + high limit
	.byte 0x0 					# base high
 
# gdt kernel data segment:		# data descriptor
	.word 0x0FFFF 				# limit low (Same as code)
	.word 0x0 					# base low
	.byte 0x0 					# base middle
	.byte 0B10010010 			# access
	.byte 0B11001111 			# granularity
	.byte 0x0					# base high
 
end_of_gdt_ap:
toc_ap: 
	.word end_of_gdt_ap - gdt_data_ap - 1 
	.long gdt_data_ap
	
.section .text
.code16
ap_trampoline_start:
    cli
	lgdt toc_ap							
	mov	%cr0,%eax		
	or	$0x0001,%eax
	mov	%eax,%cr0
    jmp	$0x08,$ap_trampoline_pmode	

.code32
ap_trampoline_pmode:
    mov	$0x10,%eax	    	
    mov	%eax,%ds
    mov	%eax,%ss
    mov	%eax,%es
    mov $0b10100000, %eax                		
    mov %eax, %cr4
    mov $PHY_MASTER_PML4,%ebx                              
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
    mov toc, %eax                    
    lgdt toc           					    
    jmp	$0x08,$ap_trampoline_lmode 

#The most common and basic method is to execute the CPUID instruction with EAX = 1.
#The APIC ID is returned in the EBX register, specifically in bits 31 to 24 (the upper 8 bits of the

.code64
ap_trampoline_lmode:
    mov	$0x10,%ax	    	
	mov	%ax,%ss
    mov $0x1, %rax
    cpuid    
    and $0xff000000, %ebx
    shr $0x18, %ebx
#case_cpu_0:
#    cmp $CPU_0, %ebx
#    jne case_cpu_1
#    mov $1, %rax
#	#mov %ax, AP_GDT_DESC_INDX
#	jmp end_switch
# We don't use CPU_0 because AP. AP_GDT_DESC index for cpu_1 is 0 because no AP desc.
case_cpu_1:
    cmp $CPU_1,  %ebx
    jne case_cpu_2
    mov $0, %rax  
	jmp end_switch
case_cpu_2:
    cmp $CPU_2, %ebx
    jne case_cpu_3
    mov $1, %rax 
	jmp end_switch
case_cpu_3:
    cmp $CPU_3, %ebx
    jne case_cpu_4
    mov $2, %rax   
	jmp end_switch
case_cpu_4:
    cmp $CPU_4, %ebx
    jne case_cpu_5
    mov $3, %rax 
	jmp end_switch
case_cpu_5:
    cmp $CPU_5, %ebx
    jne case_cpu_6
    mov $4, %rax 
	jmp end_switch
case_cpu_6:
    cmp $CPU_6, %ebx
    jne case_cpu_7
    mov $5, %rax 
	jmp end_switch
case_cpu_7:
    cmp $CPU_7, %ebx
    jne case_cpu_8
    mov $6, %rax 
	jmp end_switch
case_cpu_8:
    cmp $CPU_8, %ebx
    jne case_cpu_9
    mov $7, %rax 
	jmp end_switch
case_cpu_9:
    cmp $CPU_9, %ebx
    jne case_cpu_10
    mov $8, %ax 
	jmp end_switch
case_cpu_10:
    cmp $CPU_10, %ebx
    jne case_cpu_11
    mov $9, %rax 
	jmp end_switch
case_cpu_11:
    cmp $CPU_11, %ebx
    jne case_cpu_12
    mov $10, %rax 
	jmp end_switch
case_cpu_12:
    cmp $CPU_12, %ebx
    jne case_cpu_13
    mov $11, %rax 
	jmp end_switch
case_cpu_13:
    cmp $CPU_13, %ebx
    jne case_cpu_14
    mov $12, %rax 
	jmp end_switch
case_cpu_14:
    cmp $CPU_14, %ebx
    jne case_cpu_15
    mov $13, %ax 
	jmp end_switch
case_cpu_15:
    cmp $CPU_15, %ebx
    jne case_cpu_16
    mov $14, %rax 
	jmp end_switch
case_cpu_16:
    cmp $CPU_16, %ebx
    jne case_cpu_17
    mov $15, %rax 
	jmp end_switch
case_cpu_17:
    cmp $CPU_17, %ebx
    jne case_cpu_18
    mov $16, %rax 
	jmp end_switch
case_cpu_18:
    cmp $CPU_18, %ebx
    jne case_cpu_19
    mov $17, %rax 
	jmp end_switch
case_cpu_19:
    cmp $CPU_19, %ebx
    jne case_cpu_20
    mov $18, %rax 
	jmp end_switch
case_cpu_20:
    cmp $CPU_20, %ebx
    jne case_cpu_21
    mov $19, %rax 
	jmp end_switch
case_cpu_21:
    cmp $CPU_21, %ebx
    jne case_cpu_22
    mov $20, %rax 
	jmp end_switch
case_cpu_22:
    cmp $CPU_22, %ebx
    jne case_cpu_23
    mov $21, %rax 
	jmp end_switch
case_cpu_23:
    cmp $CPU_23,%ebx
    jne case_cpu_24
    mov $22, %ax 
	jmp end_switch
case_cpu_24:
    cmp $CPU_24, %ebx
    jne case_cpu_25
    mov $23, %rax 
	jmp end_switch
case_cpu_25:
    cmp $CPU_25, %ebx
    jne case_cpu_26
    mov $24, %rax 
	jmp end_switch
case_cpu_26:
    cmp $CPU_26, %ebx
    jne case_cpu_27
    mov $25, %rax 
	jmp end_switch
case_cpu_27:
    cmp $CPU_27, %ebx
    jne case_cpu_28
    mov $26, %rax 
	jmp end_switch
case_cpu_28:
    cmp $CPU_28, %ebx
    jne case_cpu_29
    mov $27, %rax 
	jmp end_switch
case_cpu_29:
    cmp $CPU_29, %ebx
    jne case_cpu_30
    mov $28, %rax 
	jmp end_switch
case_cpu_30:
    cmp $CPU_30, %ebx
    jne case_cpu_31
    mov $29, %rax 
	jmp end_switch
case_cpu_31:
    cmp $CPU_31, %ebx
    mov $30, %rax 

#end_switch:   
#    movabs $idt_ptr, %r9
#    mov %r9, AP_IDT
#    lidt AP_IDT
#    mov $AP_TSS, %r8
#    mov $AP_GDT, %rdx
#    mov $AP_GDT_DESC_INDX, %rbx
#    mov %rax, AP_GDT_DESC_INDX
#    mov $AP_GDT_DESC, %rcx
#    mov  AP_GDT_DESC_INDX, %rbx
#    imul $0x10, %rbx
#    add %rbx, %rcx
#    mov %rcx, AP_GDT_DESC_INDX             
#    lgdt (%rcx)           				
#    #jmp	$0x08,$end_init_ap    
#end_init_ap:
#	hlt
	
end_switch:   
    movabs $idt_ptr, %r9
    mov %r9, AP_IDT
    lidt AP_IDT
    mov $AP_GDT_DESC, %rcx
    imul $0x10, %rax
    add %rax, %rcx        
    lgdt (%rcx)           				
    #jmp	$0x08,$end_init_ap    
end_init_ap:
	hlt
ap_trampoline_end:
	hlt

