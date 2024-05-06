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
    Commands command;   // command type from tree (e.g while)

    bool imm_need;      // if command needs immediate argument
    bool reg_need;      // if command needs register argument
    bool mem_need;      // if command needs argument from memory

    Registers reg1;     // first register name (r?x)
    Registers reg2;     // second register name (r?x)
    elem_t imm;         // immediate value (elem_t is double)

    char* func_name;    // function name
};

typedef Ir* list_elem_t;

#endif
