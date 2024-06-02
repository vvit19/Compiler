# Compiler
## Description
This project is an implementation of a compiler from [my own language](https://github.com/vvit19/Language) to x86-64 executable file (ELF). Optionally it can generate ```Nasm``` assembly listing.
## Build
- Clone the repo:
```
git clone https://github.com/vvit19/Compiler.git
```
- Run code written on my language (file with code has to be in ```examples``` folder):
```
chmod +x run.sh
./run.sh filename
```
- If you want to generate assembly listing file (it's path: ```examples/filename.s```):
```
./run.sh filename -S
```

## Compiling process
1) Parsing code into AST (frontend)
2) Optimizing AST (middleend)
> Middleend and frontend stages were the same in my language project.
3) Converting AST into the intermediate representation (IR). IR is based on double-linked list, where each node has the following structure:
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

> Double-linked list was chosen in order to simplify optimizations, because it has simple nodes deletions. Currently, optimizations are not done due to author's exams at MIPT.

4) Translation of intermediate representation into ELF file.

    Generated ELF file has 3 segments:

    - service segment - it stores information about ELF headers

    - data segment - segment for data with writing and reading rights

    - text segment - segment for code with executing and reading rights

> If ```-S``` option was chosen - at this stage translation will be done into ```Nasm``` instead of ```ELF```.

## Performance test

Test consits of comparing the performance of generated ELF file and executable file from [my language project](https://github.com/vvit19/Language). The runtime is detected by ```_rdtsc()``` function.

There are 2 programs for tests, written in my language:

1) Calculating 6! for 5000000 times ([factorial](examples/factorial_bench.vit)):

| ELF file <br> CPU ticks | Out file from my language <br> CPU ticks | Boost    |
| :---------------------: | :--------------------------------------: | :------: |
|  52 906 517             |  461 926 802                             | 8,73     |

2) Solving quadratic equation $x^2 + 3x + 2 = 0$ for 5000000 times ([quadratic](examples/quadratic_bench.vit)):

| ELF file <br> CPU ticks | Out file from my language <br> CPU ticks | Boost    |
| :---------------------: | :--------------------------------------: | :------: |
|       32 926 944        |  271 504 720                             | 8,25     |


It's obvious that generated ELF file is several times faster.

## Some useful resources

- Randal Bryant, David O'Hallaron - Computer Systems: A Programmer's Perspective 3rd Edition
- ELF file format - https://ejudge.ru/study/3sem/ELF.html
- x64 opcodes - http://ref.x86asm.net/coder64-abc.html
