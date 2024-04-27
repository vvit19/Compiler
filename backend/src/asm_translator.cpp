#include "backend.h"
#include "config.h"
#include "utils.h"
#include <cassert>
#include <cstdio>

const int MAX_FUNCS = 50;

static void HandlePush   (FILE* file, Ir* ir);
static void HandlePop    (FILE* file, Ir* ir);
static void HandleMath   (FILE* file, Ir* ir);
static void HandleCall   (FILE* file, Ir* ir);
static void BeginAsmCode (FILE* file);

void TranslateToAsm (List* ir_array, const char* filename)
{
    FILE* asm_file = fopen (filename, "w");

    BeginAsmCode (asm_file);

    int cur_list_index = ir_array->head;
    ListNode* nodes = ir_array->nodes;
    ListNode cur_node = nodes[cur_list_index];

    while (cur_list_index != 0)
    {
        Ir* cur_ir = cur_node.value;

        switch (cur_ir->command)
        {
            case Commands::PUSH:  HandlePush    (asm_file, cur_ir);                                    break;
            case Commands::POP:   HandlePop     (asm_file, cur_ir);                                    break;
            case Commands::RET:   PrintCommand  (asm_file, "ret \n");                                  break;
            case Commands::CALL:  HandleCall    (asm_file, cur_ir);                                    break;

            case Commands::ADD:
            case Commands::SUB:
            case Commands::MUL:
            case Commands::DIV:   HandleMath    (asm_file, cur_ir);                                    break;
            case Commands::LABEL: fprintf       (asm_file, "L_%lld: \n", (long long) cur_ir->imm);     break;
            case Commands::FUNC:  fprintf (asm_file, "%s: \n", cur_ir->label_name);                    break;
            case Commands::MOV:   PrintCommand  (asm_file, "mov %s, %s \n",
                                                registers_strings[cur_ir->reg1],
                                                registers_strings[cur_ir->reg2]);                      break;

            case Commands::CMP:   PrintCommand  (asm_file, "cmp %s, %s \n",
                                                 registers_strings[cur_ir->reg1],
                                                 registers_strings[cur_ir->reg2]);                     break;

            case Commands::IN:    PrintCommand  (asm_file, "call in \n");                              break;
            case Commands::OUT:   PrintCommand  (asm_file, "call out \n");                             break;
            case Commands::JUMP:  PrintCommand  (asm_file, "jmp L_%lld \n",  (long long) cur_ir->imm); break;
            case Commands::JA:    PrintCommand  (asm_file, "ja L_%lld \n",   (long long) cur_ir->imm); break;
            case Commands::JAE:   PrintCommand  (asm_file, "jae L_%lld \n",  (long long) cur_ir->imm); break;
            case Commands::JB:    PrintCommand  (asm_file, "jb L_%lld \n",   (long long) cur_ir->imm); break;
            case Commands::JBE:   PrintCommand  (asm_file, "jbe L_%lld \n",  (long long) cur_ir->imm); break;
            case Commands::JE:    PrintCommand  (asm_file, "je L_%lld \n",   (long long) cur_ir->imm); break;
            case Commands::JNE:   PrintCommand  (asm_file, "jne L_%lld \n",  (long long) cur_ir->imm); break;

            default:              NO_PROPER_CASE_FOUND;
        }

        cur_list_index = cur_node.next;
        cur_node = nodes[cur_list_index];
    }

    fclose (asm_file);
}

static void HandleMath (FILE* file, Ir* ir)
{
    assert (file);
    assert (ir);

    PrintCommand (file, "movsd xmm0, qword [rsp + 8] \n");
    PrintCommand (file, "movsd xmm1, qword [rsp] \n");
    PrintCommand (file, "pop rcx\n");

    switch (ir->command)
    {
        case Commands::ADD: PrintCommand(file, "addsd xmm0, xmm1 \n"); break;
        case Commands::SUB: PrintCommand(file, "subsd xmm0, xmm1 \n"); break;
        case Commands::MUL: PrintCommand(file, "mulsd xmm0, xmm1 \n"); break;
        case Commands::DIV: PrintCommand(file, "divsd xmm0, xmm1 \n"); break;
        default: NO_PROPER_CASE_FOUND;
    }

    PrintCommand(file, "movsd qword [rsp], xmm1 \n");
}

static void HandlePush (FILE* file, Ir* ir)
{
    assert (file);
    assert (ir);

    if (ir->imm_need && ir->mem_need && ir->reg_need)
    {
        PrintCommand(file, "push qword [%s + %lld] \n", registers_strings[ir->reg1], (long long) ir->imm * ELEM_SIZE);
    }
    else if (ir->imm_need && !ir->mem_need && !ir->reg_need)
    {
        long long num = {};
        memcpy (&num, &ir->imm, sizeof (double));

        PrintCommand (file, "sub rsp, 8 \n");
        PrintCommand (file, "mov rcx, 0x%llx \n", num);
        PrintCommand (file, "mov qword [rsp], rcx \n");
    }
    else if (!ir->imm_need && !ir->mem_need && ir->reg_need)
    {
        PrintCommand(file, "push %s \n", registers_strings[ir->reg1]);
    }
}

static void HandlePop (FILE* file, Ir* ir)
{
    assert (file);
    assert (ir);

    if (ir->imm_need && ir->mem_need && ir->reg_need)
    {
        PrintCommand(file, "pop qword [%s + %d] \n", registers_strings[ir->reg1],  (int) ir->imm * ELEM_SIZE);
    }
    else if (!ir->imm_need && !ir->mem_need && ir->reg_need)
    {
        PrintCommand(file, "pop %s \n", registers_strings[ir->reg1]);
    }
    else NO_PROPER_CASE_FOUND;
}

static void HandleCall (FILE* file, Ir* ir)
{
    assert (file);

    PrintCommand (file, "add rbp, %d \n", MAX_VARS);
    PrintCommand (file, "call %s \n", ir->label_name);
    PrintCommand (file, "sub rbp, %d \n", MAX_VARS);
}

static void BeginAsmCode (FILE* file)
{
    assert (file);

    fprintf (file,
             "global _start \n"
             "section .data \n"
             "buffer: times %d dq 0 \n"
             "section .text \n"
             "_start: \n"
             "\t\tmov rbx, buffer \n"
             "\t\tmov rbp, rbx \n"
             "\t\tadd rbp, %d \n"
             "\t\tcall main \n"
             "\t\tmov eax, 1 \n"
             "\t\tmov ebx, 0 \n"
             "\t\tint 80h \n",
              MAX_VARS * MAX_FUNCS, MAX_VARS
            );
}
