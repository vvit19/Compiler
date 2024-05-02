#include "config.h"
#include "translator.h"
#include <elf.h>
#include "backend.h"

struct ElfHeaders
{
    Elf64_Ehdr elf_header;
    // Elf64_Phdr prog_header;
    Elf64_Phdr code_header;
    // Elf64_Phdr data_header;
};

void MakeElfHeader  (Elf64_Ehdr* header);
// void MakeProgHeader (Elf64_Phdr* header);
void MakeCodeHeader (Elf64_Phdr* header, size_t code_size);
// void MakeDataHeader (Elf64_Phdr* header);

void PrintHeaders (FILE* file, size_t code_size)
{
    assert (file);

    ElfHeaders headers = {};

    MakeElfHeader  (&headers.elf_header);
    // MakeProgHeader (&headers.prog_header);
    MakeCodeHeader (&headers.code_header, code_size);
    // MakeDataHeader (&headers.data_header);
    fwrite (&headers, sizeof (ElfHeaders), 1, file);
}

void MakeElfHeader (Elf64_Ehdr* header)
{
    assert (header);

    header->e_ident[EI_MAG0]  = ELFMAG0;
    header->e_ident[EI_MAG1]  = ELFMAG1;
    header->e_ident[EI_MAG2]  = ELFMAG2;
    header->e_ident[EI_MAG3]  = ELFMAG3;
    header->e_ident[EI_CLASS] = ELFCLASS64;
    header->e_ident[EI_DATA]  = ELFDATA2LSB;
    header->e_ident[EI_VERSION] = EV_CURRENT;
    header->e_ident[EI_OSABI] = ELFOSABI_NONE;
    header->e_type = ET_EXEC;
    header->e_machine = EM_X86_64;
    header->e_version = EV_CURRENT;
    header->e_entry = START_ADDRESS + sizeof (ElfHeaders) + VARIABLES_ARRAY_SIZE + STDLIB_SIZE;
    header->e_phoff = sizeof (Elf64_Ehdr);
    header->e_shoff = 0;    // no section table
    header->e_flags = 0;
    header->e_ehsize = sizeof (Elf64_Ehdr);
    header->e_phentsize = sizeof (Elf64_Phdr);
    header->e_phnum = 1;
}

void MakeCodeHeader (Elf64_Phdr* header, size_t code_size)
{
    assert (header);

    header->p_type = PT_LOAD;
    header->p_offset = 0;
    header->p_vaddr = START_ADDRESS;
    header->p_filesz = VARIABLES_ARRAY_SIZE + STDLIB_SIZE + code_size;
    header->p_memsz = header->p_filesz;
    header->p_flags = PF_X | PF_R | PF_W;
    header->p_align = PAGE_SIZE;
}
