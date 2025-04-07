#ifndef ELF_LOADER_H                
#define ELF_LOADER_H

#include "system.h"

#define EI_NIDENT (16)
#define PT_LOAD	1
#define PT_PHDR	6

//typedef unsigned short Elf32_Half;
//typedef unsigned int Elf32_Word;
//typedef	int  Elf32_Sword;
//typedef unsigned int Elf32_Addr;
//typedef unsigned int Elf32_Off;--8

typedef unsigned short Elf64_Half; 
typedef unsigned int Elf64_Word;
typedef unsigned long Elf64_Addr; 
typedef unsigned long Elf64_Off;
typedef unsigned long Elf64_Xword;

typedef struct
{
  unsigned char	e_ident[EI_NIDENT];	/* Magic number and other info */
  Elf64_Half	e_type;			/* Object file type */
  Elf64_Half	e_machine;		/* Architecture */
  Elf64_Word	e_version;		/* Object file version */
  Elf64_Addr	e_entry;		/* Entry point virtual address */
  Elf64_Off	    e_phoff;		/* Program header table file offset */
  Elf64_Off	    e_shoff;		/* Section header table file offset */
  Elf64_Word	e_flags;		/* Processor-specific flags */
  Elf64_Half	e_ehsize;		/* ELF header size in bytes */
  Elf64_Half	e_phentsize;		/* Program header table entry size */
  Elf64_Half	e_phnum;		/* Program header table entry count */
  Elf64_Half	e_shentsize;		/* Section header table entry size */
  Elf64_Half	e_shnum;		/* Section header table entry count */
  Elf64_Half	e_shstrndx;		/* Section header string table index */
} Elf64_Ehdr;

typedef struct
{
  Elf64_Word	sh_name;		/* Section name (string tbl index) */
  Elf64_Word	sh_type;		/* Section type */
  Elf64_Xword	sh_flags;		/* Section flags */
  Elf64_Addr	sh_addr;		/* Section virtual addr at execution */
  Elf64_Off	    sh_offset;		/* Section file offset */
  Elf64_Xword	sh_size;		/* Section size in bytes */
  Elf64_Word	sh_link;		/* Link to another section */
  Elf64_Word	sh_info;		/* Additional section information */
  Elf64_Xword	sh_addralign;		/* Section alignment */
  Elf64_Xword	sh_entsize;		/* Entry size if section holds table */
} Elf64_Shdr;

typedef struct
{
  Elf64_Word	p_type;			/* Segment type */
  Elf64_Word	p_flags;		/* Segment flags */
  Elf64_Off	    p_offset;		/* Segment file offset */
  Elf64_Addr	p_vaddr;		/* Segment virtual address */
  Elf64_Addr	p_paddr;		/* Segment physical address */
  Elf64_Xword	p_filesz;		/* Segment size in file */
  Elf64_Xword	p_memsz;		/* Segment size in memory */
  Elf64_Xword	p_align;		/* Segment alignment */
} Elf64_Phdr;

struct s_elf_desc
{
	u32 file_desc;
	u32 file_offset;
	u32 file_size;
}
typedef t_elf_desc;

u32 elf_loader_init(t_elf_desc* elf_desc,char* path);
u32 elf_loader_free(t_elf_desc* elf_desc);
u32 elf_loader_read(t_elf_desc* elf_desc,u64 fault_addr,u64 page_addr);


#endif
