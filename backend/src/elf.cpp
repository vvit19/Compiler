#include "config.h"
#include "translator.h"
#include <elf.h>
#include "backend.h"

const int STDLIB_SIZE = 0;


void PrintElfHeader (FILE* file)
{
    assert (file);

    Elf64_Ehdr header = {};
    header.e_ident[EI_MAG0]  = ELFMAG0;
    header.e_ident[EI_MAG1]  = ELFMAG1;
    header.e_ident[EI_MAG2]  = ELFMAG2;
    header.e_ident[EI_MAG3]  = ELFMAG3;
    header.e_ident[EI_CLASS] = ELFCLASS64;
    header.e_ident[EI_DATA]  = ELFDATA2LSB;
    header.e_ident[EI_VERSION] = EV_CURRENT;
    header.e_ident[EI_OSABI] = ELFOSABI_NONE;
    header.e_type = ET_EXEC;
    header.e_machine = EM_X86_64;
    header.e_version = EV_CURRENT;
    header.e_entry = START_ADDRESS + sizeof (Elf64_Ehdr) + sizeof (Elf64_Phdr) + sizeof (Elf64_Phdr) + VARIABLES_ARRAY_SIZE + STDLIB_SIZE;
}
