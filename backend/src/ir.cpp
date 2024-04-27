#include "backend.h"
#include "config.h"
#include "ir_config.h"
#include "lang_tree.h"

struct AsmInfo
{
    char**   names_table;
    char**   global_names_table;
    int      global_names_num;
    int      names_num;
    int      label_num;
    KeyWords prev_option;
};

static void  NodeToIr       (List* ir_list, Node* node, AsmInfo* info);
static void  PushNum        (List* ir_list, elem_t num);
static void  PushVar        (List* ir_list, const char* var, AsmInfo* info);
static void  PopVar         (List* ir_list, const char* var, AsmInfo* info);
static void  PushFuncName   (List* ir_list, Node* node, AsmInfo* info);
static void  PushFunction   (List* ir_list, Node* node, AsmInfo* info);
static void  PushNewVar     (List* ir_list, Node* node, AsmInfo* info);
static void  PushCall       (List* ir_list, Node* node, AsmInfo* info);
static void  PushRet        (List* ir_list, Node* node, AsmInfo* info);
static void  PushIn         (List* ir_list, Node* node, AsmInfo* info);
static void  PushOut        (List* ir_list, Node* node, AsmInfo* info);
static void  PushIf         (List* ir_list, Node* node, AsmInfo* info);
static void  PushElse       (List* ir_list, Node* node, AsmInfo* info);
static void  PushEq         (List* ir_list, Node* node, AsmInfo* info);
static void  PushParams     (List* ir_list, Node* node, AsmInfo* info);
static void  PushParam      (List* ir_list, Node* node, AsmInfo* info);
static void  PushCondJump   (List* ir_list, Commands jump, Node* node, AsmInfo* info);
static void  PushMathOp     (List* ir_list, Commands op,   Node* node, AsmInfo* info);
static void  PushOp         (List* ir_list, Node* node, AsmInfo* info);
static int   GetVarPosition (const char* var, char** names_array, int n_names);
static Ir*   NewIr          (Ir ir_copy);

List* AstToIr (Node* main_node)
{
    List* ir_list = (List*) calloc (1, sizeof (List));
    ListCtor (ir_list, INITIAL_LIST_CAPACITY);

    struct AsmInfo info     = {};
    info.names_table        = (char**) calloc (MAX_VARS, sizeof (char*));
    info.global_names_table = (char**) calloc (MAX_VARS, sizeof (char*));
    info.global_names_num   = 0;
    info.names_num          = 0;
    info.label_num          = 0;
    info.prev_option        = FUNC;

    NodeToIr (ir_list, main_node, &info);

    free (info.global_names_table);
    free (info.names_table);

    return ir_list;
}

static void NodeToIr (List* ir_list, Node* node, AsmInfo* info)
{
    assert (ir_list);
    assert (info);

    if (!node) return;

    if (node->type == NUM_T)
        PushNum (ir_list, node->value.num);
    else if (node->type == VAR_T)
        PushVar (ir_list, node->value.var, info);
    else if (node->type == FUNC_NAME)
        PushFuncName (ir_list, node, info);
    else if (node->type == OP_T)
        PushOp (ir_list, node, info);
}

static void PushOp (List* ir_list, Node* node, AsmInfo* info)
{
    ASSERT_BASE_PARAMS;

    switch (node->value.op)
    {
        case NEXT_OP:          CHILDREN_TO_IR;                     break;
        case FUNC:             PushFunction (ir_list, node, info); break;
        case VAR: case GL_VAR: PushNewVar   (ir_list, node, info); break;
        case CALL:             PushCall     (ir_list, node, info); break;
        case RET:              PushRet      (ir_list, node, info); break;
        case PARAM:            PushParam    (ir_list, node, info); break;
        case IN:               PushIn       (ir_list, node, info); break;
        case OUT:              PushOut      (ir_list, node, info); break;
        case IF:               PushIf       (ir_list, node, info); break;
        case ELSE:             PushElse     (ir_list, node, info); break;
        case EQ:               PushEq       (ir_list, node, info); break;
        case ISEQ:             PushCondJump (ir_list, Commands::JNE, node, info); break;
        case NEQ:              PushCondJump (ir_list, Commands::JE,  node, info); break;
        case BIGGER:           PushCondJump (ir_list, Commands::JBE,  node, info); break;
        case SMALLER:          PushCondJump (ir_list, Commands::JAE,  node, info); break;
        case BIGEQ:            PushCondJump (ir_list, Commands::JB, node, info); break;
        case SMALLEQ:          PushCondJump (ir_list, Commands::JA, node, info); break;
        case ADD:              PushMathOp   (ir_list, Commands::ADD, node, info); break;
        case SUB:              PushMathOp   (ir_list, Commands::SUB, node, info); break;
        case MULT:             PushMathOp   (ir_list, Commands::MUL, node, info); break;
        case DIV:              PushMathOp   (ir_list, Commands::DIV, node, info); break;
        // case SIN:              PushMathOp   (ir_list, Commands::ADD, node, info); break;
        // case COS:              PushMathOp   (ir_list, Commands::ADD, node, info); break;
        // case SQRT:             PushMathOp   (ir_list, Commands::ADD, node, info); break;
        // case WHILE:            PushMathOp   (ir_list, Commands::ADD, node, info); break;
        default:               NO_PROPER_CASE_FOUND;
    }
}

static void PushMathOp (List* ir_list, Commands op, Node* node, AsmInfo* info)
{
    ASSERT_BASE_PARAMS;

    CHILDREN_TO_IR;

    Ir* ir = NewIr({op, NO_ARGS, NO, NO, NO, nullptr});
    INSERT;
}

static void PushEq (List* ir_list, Node* node, AsmInfo* info)
{
    ASSERT_BASE_PARAMS;

    RIGHT_CHILD_TO_IR;

    Ir* ir = nullptr;

    if (node->right->type == OP_T && node->right->value.op == CALL)
    {
        ir = NewIr ({Commands::PUSH, REG_NEED, RAX, NO, 0, nullptr});
        INSERT;
    }

    if (node->left && node->left->type == VAR_T)
        PopVar (ir_list, node->left->value.var, info);
}

static void PushIf (List* ir_list, Node* node, AsmInfo* info)
{
    ASSERT_BASE_PARAMS;

    CHILDREN_TO_IR;

    Ir* ir = NewIr ({Commands::LABEL, IMM_NEED, NO, NO, (elem_t) info->label_num, nullptr});
    INSERT;

    ++info->label_num;
}

static void PushElse (List* ir_list, Node* node, AsmInfo* info)
{
    ASSERT_BASE_PARAMS;

    LEFT_CHILD_TO_IR;
    int else_label = info->label_num++;

    NodeToIr (ir_list, node->right->left, info);

    Ir* ir = NewIr ({Commands::JUMP, IMM_NEED, NO, NO, (elem_t) info->label_num, nullptr});
    INSERT;
    int after_label = info->label_num;

    ir = NewIr ({Commands::LABEL, IMM_NEED, NO, NO, (elem_t) else_label, nullptr});
    INSERT;

    NodeToIr (ir_list, node->right->right, info);

    ir = NewIr ({Commands::LABEL, IMM_NEED, NO, NO, (elem_t) after_label, nullptr});
    INSERT;
}

static void PushParam (List* ir_list, Node* node, AsmInfo* info)
{
    ASSERT_BASE_PARAMS;

    Ir* ir = nullptr;

    switch (info->prev_option)
    {
        case IN:
            ir = NewIr ({Commands::IN, NO_ARGS, NO, NO, 0, nullptr});
            INSERT;
            ir = NewIr ({Commands::PUSH, REG_NEED, RAX, NO, 0, nullptr});
            INSERT;
            PopVar (ir_list, node->left->value.var, info);
            break;

        case OUT:
            if (node->left->type == NUM_T)
            {
                ir = NewIr ({Commands::PUSH, IMM_NEED, NO, NO, node->left->value.num, nullptr});
                INSERT;
            }
            else
            {
                PushVar (ir_list, node->left->value.var, info);
            }

            ir = NewIr ({Commands::OUT, NO_ARGS, NO, NO, 0, nullptr});
            INSERT;
            break;

        case FUNC:
            info->names_table[info->names_num] = node->left->value.var;
            ir = NewIr({Commands::POP, NEED_ALL, RBP, NO, (elem_t) info->names_num, nullptr});
            INSERT;
            ++info->names_num;
            break;

        case RET:
            CHILDREN_TO_IR;
            ir = NewIr({Commands::POP, REG_NEED, RAX, NO, 0, nullptr});
            INSERT;
            return;

        default:
            NO_PROPER_CASE_FOUND;
    }

    RIGHT_CHILD_TO_IR;
}

static void PushRet (List* ir_list, Node* node, AsmInfo* info)
{
    ASSERT_BASE_PARAMS;

    info->prev_option = RET;
    LEFT_CHILD_TO_IR;

    Ir* ir = NewIr({Commands::PUSH, REG_NEED, RSI, NO, 0, nullptr});
    INSERT;

    ir = NewIr ({Commands::RET, NO_ARGS, NO, NO, 0, nullptr});
    INSERT;
}

static void PushIn (List* ir_list, Node* node, AsmInfo* info)
{
    ASSERT_BASE_PARAMS;

    info->prev_option = IN;
    LEFT_CHILD_TO_IR;
}

static void  PushOut (List* ir_list, Node* node, AsmInfo* info)
{
    ASSERT_BASE_PARAMS;

    info->prev_option = OUT;
    LEFT_CHILD_TO_IR;
}

static void PushCondJump (List* ir_list, Commands jump, Node* node, AsmInfo* info)
{
    ASSERT_BASE_PARAMS;

    CHILDREN_TO_IR;

    Ir* ir = NewIr ({Commands::POP, REG_NEED, RCX, NO, 0, nullptr});
    INSERT;

    ir = NewIr ({Commands::POP, REG_NEED, RDX, NO, 0, nullptr});
    INSERT;

    ir = NewIr ({Commands::CMP, REG_NEED, RCX, RDX, 0, nullptr});
    INSERT;

    ir = NewIr ({jump, IMM_NEED, NO, NO, (elem_t) info->label_num, nullptr});
    INSERT;
}

static void PushCall (List* ir_list, Node* node, AsmInfo* info)
{
    ASSERT_BASE_PARAMS;

    Ir* ir = NewIr ({Commands::PUSH, REG_NEED, RSI, NO, 0, nullptr});
    INSERT;

    info->prev_option = CALL;

    PushParams (ir_list, node->right, info);

    ir = NewIr ({Commands::CALL, false, false, false, NO, NO, 0, node->left->value.var});
    INSERT;

    ir = NewIr ({Commands::POP, REG_NEED, RSI, NO, 0, nullptr});
    INSERT;
}

static void PushParams (List* ir_list, Node* node, AsmInfo* info)
{
    if (node->right) PushParams (ir_list, node->right, info);

    LEFT_CHILD_TO_IR;
}

static void PushNewVar (List* ir_list, Node* node, AsmInfo* info)
{
    ASSERT_BASE_PARAMS;

    RIGHT_CHILD_TO_IR;

    Ir* ir = nullptr;

    if (node->value.op == VAR)
    {
        info->names_table[info->names_num] = node->left->value.var;
        ir = NewIr ({Commands::POP, NEED_ALL, RBP, NO, (elem_t) info->names_num});
        info->names_num++;
    }
    else
    {
        info->global_names_table[info->global_names_num] = node->left->value.var;
        ir = NewIr ({Commands::POP, NEED_ALL, RBX, NO, (elem_t) info->global_names_num});
        info->global_names_num++;
    }

    INSERT;
}

static void PushFunction (List* ir_list, Node* node, AsmInfo* info)
{
    ASSERT_BASE_PARAMS;

    free (info->names_table);

    info->names_table = (char**) calloc (MAX_VARS, sizeof (char*));
    info->names_num = 0;
    info->prev_option = FUNC;

    CHILDREN_TO_IR;
}

static void PushFuncName (List* ir_list, Node* node, AsmInfo* info)
{
    ASSERT_BASE_PARAMS;

    Ir* ir = NewIr ({Commands::FUNC, NO_ARGS, NO, NO, 0, node->value.var});
    INSERT;

    ir = NewIr({Commands::POP, REG_NEED, RSI, NO, 0, nullptr});
    INSERT;

    CHILDREN_TO_IR;
}

static void PushNum  (List* ir_list, elem_t num)
{
    assert (ir_list);

    Ir* ir = NewIr ({Commands::PUSH, true, false, false, NO, NO, num, nullptr});
    INSERT;
}

static void PushVar (List* ir_list, const char* var, AsmInfo* info)
{
    assert (ir_list);
    assert (var);
    assert (info);

    Ir* ir = nullptr;

    int position = GLOBAL_VAR_POS (var);

    if (position >= 0)
        ir = NewIr ({Commands::PUSH, true, true, true, RBX, NO, (elem_t) position, nullptr});
    else
        ir = NewIr ({Commands::PUSH, true, true, true, RBP, NO, (elem_t) VAR_POS(var), nullptr});

    INSERT;
}

static void PopVar (List* ir_list, const char* var, AsmInfo* info)
{
    assert (ir_list);
    assert (var);
    assert (info);

    Ir* ir = nullptr;

    int position = GLOBAL_VAR_POS (var);

    if (position >= 0)
        ir = NewIr({Commands::POP, NEED_ALL, RBX, NO, (elem_t) position, nullptr});
    else
        ir = NewIr({Commands::POP, NEED_ALL, RBP, NO, (elem_t) VAR_POS(var), nullptr});

    INSERT;
}

static int GetVarPosition (const char* var, char** names_array, int n_names)
{
    assert (var);
    assert (names_array);

    for (int i = 0; i < n_names; i++)
    {
        if (strcmp (var, names_array[i]) == 0) return i;
    }

    return -1;
}

static Ir* NewIr (Ir ir_copy)
{
    Ir* ir = (Ir*) calloc (1, sizeof (Ir));
    *ir = ir_copy;

    return ir;
}
