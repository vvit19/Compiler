# Compiler
## Description
This project is an implementation of compiler from [my own language](https://github.com/vvit19/Language) to x86-64 executable file (Elf). Optionally it can generate assembly (```nasm```) listing.
## Build
- Clone the repo:
```
git clone https://github.com/vvit19/Compiler
```

- Run code written on my language (file with code has to be in ```examples``` folder):
```
bash run.sh filename
```
- If you want to generate assembly listing file (it's path: ```examples/filename.s```):
```
bash run.sh filename -S
```
## Compiling process
1) Parsing code to tree (frontend) and optimizing it (middleend) - these stages were the same in my language
2) Converting tree to an intermediate representation (IR) - it is [double-linked list](https://github.com/vvit19/List), which elements are structures:
``` C++
struct Ir
{
    Commands command;   // command type from AST (e.g. while)

    bool imm_need;      // if command needs immediate argument
    bool reg_need;      // if command needs register argument
    bool mem_need;      // if command needs argument from memory

    Registers reg1;     // first register name (r?x)
    Registers reg2;     // second register name (r?x)
    elem_t imm;         // immediate value (elem_t is double)

    char* func_name;    // function name
};
```

3) Ir optimization.

    I have chosen double-linked list, because it can help in optimizations, due to simple list node deletions. I optimized structures like this (well, I hope I'll do this soon):

``` asm
Not optimized                               Optimized

push [rbp + 8]                              mov rcx, [rbp + 8]
pop rcx
```

4)  Translation of intermediate representation into Elf file.

    I have simple elf file format with 3 segments:

    - service segment - it stores information about elf headers

    - data segment - segment for data with writing and reading rights

    - text segment - segment for code with executing and reading rights

    > If ```-S``` option was chosen - at this stage the translation will be done into ```NASM```, not into ```Elf```.

## Performance test
