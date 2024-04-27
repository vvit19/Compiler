#ifndef CONFIG_H
#define CONFIG_H

const char* const TREE_TXT = "../middleend/tree.txt";

const int MAX_VARS = 50;
const int ELEM_SIZE = 8;

typedef double elem_t;

enum class Commands
{
    PUSH,
    POP,
    RET,
    CALL,
    ADD,
    SUB,
    MUL,
    DIV,
    LABEL,
    FUNC,
    MOV,
    IN,
    OUT,
    CMP,
    JUMP,
    JNE,
    JE,
    JAE,
    JBE,
    JA,
    JB,
    SQRT
};

enum Registers
{
    NO,
    RAX,
    RBX,
    RCX,
    RDX,
    RDI,
    RSI,
    RSP,
    RBP
};

const char* const registers_strings[] = {"\0", "rax", "rbx", "rcx", "rdx", "rdi", "rsi", "rsp", "rbp"};

struct Ir
{
    Commands command;

    bool imm_need;
    bool reg_need;
    bool mem_need;

    Registers reg1;
    Registers reg2;
    elem_t imm;

    char* label_name;
};

typedef Ir* list_elem_t;

#endif
