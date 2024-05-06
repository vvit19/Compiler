#ifndef CONFIG_H
#define CONFIG_H

#include <cstddef>

const char* const TREE_TXT = "../middleend/tree.txt";

const size_t MAX_VARS = 32;
const size_t ELEM_SIZE = 8;
const size_t MAX_VARS_LEN = 256;
const size_t VARIABLES_ARRAY_SIZE = 8192; // MAX_VARS * ELEM_SIZE * MAX_FUNCS

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
    SQRT,
};

enum Registers
{
    NO = -1,
    RAX,
    RCX,
    RDX,
    RBX,
    RSP,
    RBP,
    RSI,
    RDI
};

const char* const registers_strings[] = {"rax", "rcx", "rdx", "rbx", "rsp", "rbp", "rsi", "rdi"};

struct Ir
{
    Commands command;

    bool imm_need;
    bool reg_need;
    bool mem_need;

    Registers reg1;
    Registers reg2;
    elem_t imm;

    char* func_name;
};

typedef Ir* list_elem_t;

#endif
