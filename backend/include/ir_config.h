#ifndef IR_CONFIG_H
#define IR_CONFIG_H

#define ASSERT_BASE_PARAMS \
        assert (ir_list); \
        assert (node); \
        assert (info)

#define GLOBAL_VAR_POS(var) GetVarPosition (var, info->global_names_table, info->global_names_num)
#define VAR_POS(var) GetVarPosition (var, info->names_table, info->names_num)

#define LEFT_CHILD_TO_IR NodeToIr (ir_list, node->left, info )
#define RIGHT_CHILD_TO_IR NodeToIr (ir_list, node->right, info)

#define CHILDREN_TO_IR \
    LEFT_CHILD_TO_IR; \
    RIGHT_CHILD_TO_IR;

#define INSERT InsertTail (ir_list, ir)

#define NO_ARGS   false, false, false
#define IMM_NEED  true,  false, false
#define REG_NEED  false, true,  false
#define MEM_NEED  false, false, true
#define IMMandREG true, true, false
#define IMMandMEM true, false, true
#define REGandMEM false, true, true
#define NEED_ALL  true, true, true

#endif
