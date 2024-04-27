#include "middle.h"

int main (int argc, const char** argv)
{
    Node* main_node = GetTree (TREE_TXT);

    Optimize (&main_node);
    ASTDump  (main_node);

    TreeDtor (main_node);
}
