#ifndef TRANSLATOR_H
#define TRANSLATOR_H

#include "config.h"
#include "cstdio"
#include <cstddef>
#include <cstdint>

#define CUR_BUF_ADDRESS &translator->buffer[translator->buffer_size]

const char   FIRST_PASS = 1;
const char   SECOND_PASS = 2;
const size_t PAGE_SIZE = 4096;
const size_t N_PAGES   = 2;
const size_t START_ADDRESS = 0x400000;
const size_t MAX_JUMPS = 200;
const size_t MAX_OPCODE_LEN = 16;
const size_t STDLIB_SIZE = 333;
const size_t IN_PTR = START_ADDRESS + VARIABLES_ARRAY_SIZE;
const size_t OUT_PTR = IN_PTR + 176;
const size_t MAIN_PTR = START_ADDRESS + VARIABLES_ARRAY_SIZE + STDLIB_SIZE;

const char* const STDLIB = "../examples/stdlib_bin";

enum class CommandsX86
{
    PUSH_REG_IMM_MEM = 0,
    POP_REG_IMM_MEM,
    SUB_REG_IMM,
    ADD_REG_IMM,
    MOV_REG_IMM,
    MOV_MEMRSP_RCX,
    PUSH_REG,
    POP_REG,
    RET,
    CALL,
    MOV_XMM0_RSP8,
    MOV_XMM1_RSP,
    ADD_XMM0_XMM1,
    SUB_XMM0_XMM1,
    MUL_XMM0_XMM1,
    DIV_XMM0_XMM1,
    MOV_RSP_XMM0,
    MOV_REGS,
    CMP_REGS,
    JE,
    JNE,
    JL,
    JG,
    JLE,
    JGE,
    JMP,
    SQRT,
};

struct JumpInfo
{
    unsigned char* address;
    char* func_name;
    int label_num;
};

struct TranslatorInfo
{
    unsigned char* buffer;
    size_t buffer_size;
    JumpInfo* jump_table;
    JumpInfo* call_table;
    size_t n_calls;
    char pass_number;
};

struct OpCode
{
    CommandsX86 command;
    uint8_t opcode[MAX_OPCODE_LEN];
    size_t len;
};

const OpCode OPCODES[]
{
    {CommandsX86::PUSH_REG_IMM_MEM, {0xFF, 0xB0, 0x00, 0x00, 0x00, 0x00}, 6},
    {CommandsX86::POP_REG_IMM_MEM,  {0x8F, 0x80, 0x00, 0x00, 0x00, 0x00}, 6},
    {CommandsX86::SUB_REG_IMM,      {0x48, 0x81, 0xE8, 0x00, 0x00, 0x00, 0x00}, 7},
    {CommandsX86::ADD_REG_IMM,      {0x48, 0x81, 0xC0, 0x00, 0x00, 0x00, 0x00}, 7},
    {CommandsX86::MOV_REG_IMM,      {0x48, 0xB8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 10},
    {CommandsX86::MOV_MEMRSP_RCX,   {0x48, 0x89, 0x0C, 0x24}, 4},
    {CommandsX86::PUSH_REG,         {0x50}, 1},
    {CommandsX86::POP_REG,          {0x58}, 1},
    {CommandsX86::RET,              {0xC3}, 1},
    {CommandsX86::CALL,             {0xE8, 0x00, 0x00, 0x00, 0x00}, 5},
    {CommandsX86::MOV_XMM0_RSP8,    {0xF2, 0x0F, 0x10, 0x44, 0x24, 0x08}, 6},
    {CommandsX86::MOV_XMM1_RSP,     {0xF2, 0x0F, 0x10, 0x0C, 0x24}, 5},
    {CommandsX86::ADD_XMM0_XMM1,    {0xF2, 0x0F, 0x58, 0xC1}, 4},
    {CommandsX86::SUB_XMM0_XMM1,    {0xF2, 0x0F, 0x5C, 0xC1}, 4},
    {CommandsX86::MUL_XMM0_XMM1,    {0xF2, 0x0F, 0x59, 0xC1}, 4},
    {CommandsX86::DIV_XMM0_XMM1,    {0xF2, 0x0F, 0x5E, 0xC1}, 4},
    {CommandsX86::MOV_RSP_XMM0,     {0xF2, 0x0F, 0x11, 0x04, 0x24}, 5},
    {CommandsX86::MOV_REGS,         {0x48, 0x89, 0b11000000}, 3},
    {CommandsX86::CMP_REGS,         {0x48, 0x39, 0b11000000}, 3},
    {CommandsX86::JE,               {0x0F, 0x84, 0x00, 0x00, 0x00, 0x00}, 6},
    {CommandsX86::JNE,              {0x0F, 0x85, 0x00, 0x00, 0x00, 0x00}, 6},
    {CommandsX86::JL,               {0x0F, 0x8C, 0x00, 0x00, 0x00, 0x00}, 6},
    {CommandsX86::JG,               {0x0F, 0x8F, 0x00, 0x00, 0x00, 0x00}, 6},
    {CommandsX86::JLE,              {0x0F, 0x8E, 0x00, 0x00, 0x00, 0x00}, 6},
    {CommandsX86::JGE,              {0x0F, 0x8D, 0x00, 0x00, 0x00, 0x00}, 6},
    {CommandsX86::JMP,              {0xE9, 0x00, 0x00, 0x00, 0x00}, 5},
    {CommandsX86::SQRT,             {0xF2, 0x0F, 0x51, 0xC1}, 4},
};

void PrintHeaders (FILE* file, size_t code_size);

#endif
