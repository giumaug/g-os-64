.global out,in,outw,inw,indw,outdw,adjust_kernel_stack,syscall

# void out(unsigned char value,unsigned int address)
out:
    push %rbp
    mov %rsp,%rbp
# Da calling convention non dovrebbe essere necessario preservare edx e eax (vedi OSDev)
# Verificare cosa succede disassemblando kernel.bin   
#   push %edx
#   push %eax
	mov %edi,%edx # address
	mov %esi,%eax # value
    out %al,%dx
#   pop %eax
#   pop %edx
    pop %rbp
    ret
     
# unsigned char in(unsigned int address)
in:
	push %rbp
    mov %rsp,%rbp
#   push %edx
    mov %edi,%edx # address
    in %dx,%al
#   pop %edx
    pop %rbp
    ret

# void out(unsigned short value,unsigned int address)
outw:
	push   %rbp
 	mov    %rsp,%rbp
#	push   %edx
#	push   %eax
 	mov    %edi,%edx  # address
 	mov    %esi,%eax  # value
 	out    %ax,%dx
#	pop    %eax
#	pop    %edx
 	pop    %rbp
 	ret

# unsigned short in(unsigned int address)
inw:
	push   %rbp
 	mov    %rsp,%rbp
#	push   %edx
	mov    $0x0,%eax
 	mov    %edi,%edx  # address
 	in     %dx,%ax
#	pop    %edx
 	pop    %rbp
 	ret

# void outdw(unsigend int value,unsigned int address)
outdw:
	push   %rbp
 	mov    %rsp,%rbp
#	push   %edx
#	push   %eax
 	mov    %edi,%edx  # address
 	mov    %esi,%eax  # value
 	outl   %eax,%dx
#	pop    %eax
#	pop    %edx
 	pop    %rbp
 	ret

# unsigned int indw(unsigned int address)
indw:
	push   %rbp
 	mov    %rsp,%rbp
#	push   %edx
	mov    $0x0,%eax
 	mov    %edi,%edx  # address
 	inl    %dx,%eax
#	pop    %edx
 	pop    %rbp
 	ret         
