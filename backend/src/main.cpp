#include "backend.h"

int main (int argc, const char** argv)
{
    Node* main_node = GetTree (TREE_TXT);

    List* ir_array = AstToIr (main_node);

    TranslateToAsm (ir_array, argv[1]);
}
