#include "backend.h"
#include "config.h"
#include "translator.h"

static void FillTranslatorInfo     (TranslatorInfo* translator, List* ir_array);
static void BeginCode              (TranslatorInfo* translator);
static void HandlePush             (TranslatorInfo* translator, Ir* ir);
static void HandlePop              (TranslatorInfo* translator, Ir* ir);
static void HandleCall             (TranslatorInfo* translator, Ir* ir);
static void HandleMath             (TranslatorInfo* translator, Ir* ir);
static void HandleInOut            (TranslatorInfo* translator, Ir* ir);
static void HandleLabel            (TranslatorInfo* translator, Ir* ir);
static void HandleFunc             (TranslatorInfo* translator, Ir* ir);
static void HandleTwoRegsOp        (TranslatorInfo* translator, Ir* ir);
static void HandleJump             (TranslatorInfo* translator, Ir* ir);
static void HandleSqrt             (TranslatorInfo* translator, Ir* ir);
static void StackRegImmMem         (TranslatorInfo* translator, Registers reg, int offset, Commands command);
static void PushMathRegImm         (TranslatorInfo* translator, Registers reg, int num, Commands command);
static void MovRegImm              (TranslatorInfo* translator, Registers reg, long long num);
static void StackReg               (TranslatorInfo* translator, Registers reg, Commands command);
static void OpcodeToBuffer         (TranslatorInfo* translator, CommandsX86 command);
static void PrintStdlibCode        (FILE* file);
static unsigned char* FindFunction (TranslatorInfo* translator, char* func_name);

void TranslateToElf (List* ir_array, const char* filename)
{
    assert (ir_array);
    assert (filename);

    TranslatorInfo translator = {};
    translator.buffer         = (unsigned char*) calloc (PAGE_SIZE * N_PAGES, sizeof (char));
    translator.call_table     = (JumpInfo*)      calloc (MAX_JUMPS, sizeof (JumpInfo));
    translator.jump_table     = (JumpInfo*)      calloc (MAX_JUMPS, sizeof (JumpInfo));

    translator.pass_number    = FIRST_PASS;
    FillTranslatorInfo (&translator, ir_array);     // first pass

    translator.pass_number = SECOND_PASS;
    translator.buffer_size = 0;
    FillTranslatorInfo (&translator, ir_array);     // second pass

    FILE* file = GetFile (filename, "wb");
    PrintHeaders (file, translator.buffer_size);

    char* variables = (char*) calloc (VARIABLES_ARRAY_SIZE, sizeof (char));
    fwrite (variables, VARIABLES_ARRAY_SIZE, sizeof (char), file);

    PrintStdlibCode (file);
    fwrite (translator.buffer, sizeof (unsigned char), translator.buffer_size, file);

    fclose (file);

    free (translator.buffer);
    free (translator.call_table);
    free (translator.jump_table);
    free (variables);
}

static void FillTranslatorInfo (TranslatorInfo* translator, List* ir_array)
{
    assert (translator);
    assert (ir_array);

    BeginCode (translator);

    int cur_list_index = ir_array->head;
    ListNode* nodes = ir_array->nodes;
    ListNode cur_node = nodes[cur_list_index];

    while (cur_list_index != 0)
    {
        Ir* cur_ir = cur_node.value;

        switch (cur_ir->command)
        {
            case Commands::PUSH:  HandlePush (translator, cur_ir);               break;
            case Commands::POP:   HandlePop  (translator, cur_ir);               break;
            case Commands::RET:   OpcodeToBuffer (translator, CommandsX86::RET); break;
            case Commands::CALL:  HandleCall (translator, cur_ir);               break;
            case Commands::ADD:
            case Commands::SUB:
            case Commands::MUL:
            case Commands::DIV:   HandleMath  (translator, cur_ir);              break;
            case Commands::LABEL: if (translator->pass_number == FIRST_PASS)
                                     { HandleLabel (translator, cur_ir); }       break;
            case Commands::FUNC:  if (translator->pass_number == FIRST_PASS)
                                     { HandleFunc (translator, cur_ir); }        break;
            case Commands::MOV:
            case Commands::CMP:   HandleTwoRegsOp (translator, cur_ir);          break;
            case Commands::IN:
            case Commands::OUT:   HandleInOut (translator, cur_ir);              break;
            case Commands::JUMP:
            case Commands::JA:
            case Commands::JAE:
            case Commands::JB:
            case Commands::JBE:
            case Commands::JE:
            case Commands::JNE:   HandleJump (translator, cur_ir);               break;
            case Commands::SQRT:  HandleSqrt (translator, cur_ir);               break;
            default: NO_PROPER_CASE_FOUND;
        }

        cur_list_index = cur_node.next;
        cur_node = nodes[cur_list_index];
    }

}

static void BeginCode (TranslatorInfo* translator)
{
    assert (translator);

    unsigned char opcode[] = { 0x48, 0xC7, 0xC3, 0x00, 0x10, 0x40, 0x00, // mov rbx, variables
                               0x48, 0x89, 0xDD,                         // mov rbp, rbx
                               0x48, 0x81, 0xC5, 0x00, 0x01, 0x00, 0x00, // add rbp, MAX_VARS_LEN
                               0xE8, 0x0C, 0x00, 0x00, 0x00,             // call main
                               0xB8, 0x01, 0x00, 0x00, 0x00,             // mov eax, 1
                               0xBB, 0x00, 0x00, 0x00, 0x00,             // mov ebx, 0
                               0xCD, 0x80 };                             // int 80h

    size_t opcode_size = sizeof (opcode);

    memcpy (CUR_BUF_ADDRESS, &opcode, opcode_size);
    translator->buffer_size += opcode_size;
}

static void HandlePush (TranslatorInfo* translator, Ir* ir)
{
    assert (translator);
    assert (ir);

    if (ir->imm_need && ir->mem_need && ir->reg_need)
    {
        StackRegImmMem (translator, ir->reg1, (int) ir->imm * ELEM_SIZE, Commands::PUSH);
    }
    else if (ir->imm_need && !ir->mem_need && !ir->reg_need)
    {
        long long num = 0;
        memcpy (&num, &ir->imm, sizeof (double));

        PushMathRegImm (translator, RSP, 8, Commands::SUB);
        MovRegImm (translator, RCX, num);
        OpcodeToBuffer (translator, CommandsX86::MOV_MEMRSP_RCX);
    }
    else if (!ir->imm_need && !ir->mem_need && ir->reg_need)
    {
        StackReg (translator, ir->reg1, Commands::PUSH);
    }
}

static void HandleFunc (TranslatorInfo* translator, Ir* ir)
{
    assert (translator);
    assert (ir);

    JumpInfo* func = &translator->call_table[translator->n_calls];
    func->func_name = ir->func_name;
    ++translator->n_calls;
    func->address = CUR_BUF_ADDRESS;
}

static void HandlePop (TranslatorInfo* translator, Ir* ir)
{
    assert (translator);
    assert (ir);

    if (ir->imm_need && ir->mem_need && ir->reg_need)
    {
        StackRegImmMem (translator, ir->reg1, (int) ir->imm * ELEM_SIZE, Commands::POP);
    }
    else if (!ir->imm_need && !ir->mem_need && ir->reg_need)
    {
        StackReg (translator, ir->reg1, Commands::POP);
    }
}

static void HandleJump (TranslatorInfo* translator, Ir* ir)
{
    assert (translator);
    assert (ir);

    switch (ir->command)
    {
        case Commands::JE:   OpcodeToBuffer (translator, CommandsX86::JE);  break;
        case Commands::JNE:  OpcodeToBuffer (translator, CommandsX86::JNE); break;
        case Commands::JB:   OpcodeToBuffer (translator, CommandsX86::JL);  break;
        case Commands::JA:   OpcodeToBuffer (translator, CommandsX86::JG);  break;;
        case Commands::JAE:  OpcodeToBuffer (translator, CommandsX86::JGE); break;
        case Commands::JBE:  OpcodeToBuffer (translator, CommandsX86::JLE); break;
        case Commands::JUMP: OpcodeToBuffer (translator, CommandsX86::JMP); break;

        default: NO_PROPER_CASE_FOUND;
    }

    if (translator->pass_number == FIRST_PASS) return;

    unsigned char* jump_address = translator->jump_table[(int) ir->imm].address;
    *((int*) (CUR_BUF_ADDRESS - 4)) = jump_address - CUR_BUF_ADDRESS;
}

static void HandleMath (TranslatorInfo* translator, Ir* ir)
{
    assert (translator);
    assert (ir);

    OpcodeToBuffer (translator, CommandsX86::MOV_XMM0_RSP8);
    OpcodeToBuffer (translator, CommandsX86::MOV_XMM1_RSP);
    StackReg (translator, RCX, Commands::POP);

    switch (ir->command)
    {
        case Commands::ADD: OpcodeToBuffer (translator, CommandsX86::ADD_XMM0_XMM1); break;
        case Commands::SUB: OpcodeToBuffer (translator, CommandsX86::SUB_XMM0_XMM1); break;
        case Commands::MUL: OpcodeToBuffer (translator, CommandsX86::MUL_XMM0_XMM1); break;
        case Commands::DIV: OpcodeToBuffer (translator, CommandsX86::DIV_XMM0_XMM1); break;
        default: NO_PROPER_CASE_FOUND;
    }

    OpcodeToBuffer(translator, CommandsX86::MOV_RSP_XMM0);
}

static void HandleLabel (TranslatorInfo* translator, Ir* ir)
{
    assert (translator);
    assert (ir);

    JumpInfo* label = &translator->jump_table[(int) ir->imm];
    label->address = CUR_BUF_ADDRESS;
}

static void HandleTwoRegsOp (TranslatorInfo* translator, Ir* ir)
{
    assert (translator);
    assert (ir);

    Commands command = ir->command;

    if (command == Commands::MOV)
        OpcodeToBuffer (translator, CommandsX86::MOV_REGS);
    else
        OpcodeToBuffer (translator, CommandsX86::CMP_REGS);

    translator->buffer[translator->buffer_size - 1] |= ir->reg1 | (ir->reg2 << 3);
}

static void HandleInOut (TranslatorInfo* translator, Ir* ir)
{
    assert (translator);
    assert (ir);

    OpcodeToBuffer (translator, CommandsX86::CALL);

    if (ir->command == Commands::IN)
    {
        *((int*) (CUR_BUF_ADDRESS - sizeof (int))) = IN_PTR - MAIN_PTR - translator->buffer_size;
    }
    else
    {
        *((int*) (CUR_BUF_ADDRESS - sizeof (int))) = OUT_PTR - MAIN_PTR - translator->buffer_size;
    }
}

static void HandleCall (TranslatorInfo* translator, Ir* ir)
{
    assert (translator);
    assert (ir);

    PushMathRegImm (translator, RBP, MAX_VARS_LEN, Commands::ADD);

    OpcodeToBuffer (translator, CommandsX86::CALL);

    if (translator->pass_number == SECOND_PASS)
    {
        unsigned char* call_address = FindFunction (translator, ir->func_name);
        *((int*) (CUR_BUF_ADDRESS - sizeof (int))) = call_address - CUR_BUF_ADDRESS;
    }

    PushMathRegImm (translator, RBP, MAX_VARS_LEN, Commands::SUB);
}

static void HandleSqrt (TranslatorInfo* translator, Ir* ir)
{
    assert (translator);
    assert (ir);

    OpcodeToBuffer (translator, CommandsX86::MOV_XMM1_RSP);
    OpcodeToBuffer (translator, CommandsX86::SQRT);
    OpcodeToBuffer (translator, CommandsX86::MOV_RSP_XMM0);
}

static void StackRegImmMem (TranslatorInfo* translator, Registers reg, int offset, Commands command)
{
    assert (translator);

    if (command == Commands::PUSH)
        OpcodeToBuffer (translator, CommandsX86::PUSH_REG_IMM_MEM);
    else
        OpcodeToBuffer (translator, CommandsX86::POP_REG_IMM_MEM);

    translator->buffer[translator->buffer_size - 5] += reg;
    *((int*) (CUR_BUF_ADDRESS - sizeof (int))) = offset;
}

static void PushMathRegImm (TranslatorInfo* translator, Registers reg, int num, Commands command)
{
    assert (translator);

    if (command == Commands::SUB)
        OpcodeToBuffer (translator, CommandsX86::SUB_REG_IMM);
    else
    {
        OpcodeToBuffer (translator, CommandsX86::ADD_REG_IMM);
    }

    translator->buffer[translator->buffer_size - 5] += reg;
    *((int*) (CUR_BUF_ADDRESS - sizeof (int))) = num;
}

static void MovRegImm (TranslatorInfo* translator, Registers reg, long long num)
{
    assert (translator);

    OpcodeToBuffer (translator, CommandsX86::MOV_REG_IMM);
    translator->buffer[translator->buffer_size - 9] += reg;
    *((long long*) (CUR_BUF_ADDRESS - sizeof (long long))) = num;
}

static void StackReg (TranslatorInfo* translator, Registers reg, Commands command)
{
    assert (translator);

    if (command == Commands::PUSH)
        OpcodeToBuffer (translator, CommandsX86::PUSH_REG);
    else
        OpcodeToBuffer (translator, CommandsX86::POP_REG);

    translator->buffer[translator->buffer_size - 1] += reg;
}

static void OpcodeToBuffer (TranslatorInfo* translator, CommandsX86 command)
{
    assert (translator);

    memcpy (CUR_BUF_ADDRESS, OPCODES[(int) command].opcode, OPCODES[(int) command].len);
    translator->buffer_size += OPCODES[(int) command].len;
}

static unsigned char* FindFunction (TranslatorInfo* translator, char* func_name)
{
    assert (translator);
    assert (func_name);

    JumpInfo* call_table = translator->call_table;
    size_t size = translator->n_calls;

    for (size_t i = 0; i < size; ++i)
    {
        if (strcmp (func_name, call_table[i].func_name) == 0)
            return call_table[i].address;
    }

    return nullptr;
}

static void PrintStdlibCode (FILE* file)
{
    assert (file);

    unsigned char* buffer = (unsigned char*) calloc (STDLIB_SIZE, sizeof (unsigned char));

    FILE* stdlib_file = GetFile (STDLIB, "rb");

    fread (buffer, STDLIB_SIZE, sizeof (unsigned char), stdlib_file);
    fclose (stdlib_file);

    fwrite (buffer, STDLIB_SIZE, sizeof (unsigned char), file);

    free (buffer);
}
