#include "backend.h"
#include "lang_tree.h"

int main (int argc, const char** argv)
{
    Translation mode = HandleCommandLineArgs (argc, argv);
    if (mode == Translation::ERR)
    {
        fprintf (stderr, "Incorrect command line arguments! \n");
        return 0;
    }

    Node* main_node = GetTree (TREE_TXT);
    List* ir_array = AstToIr (main_node);

    if (mode == Translation::ASM)
        TranslateToAsm (ir_array, argv[2]);
    else
        TranslateToElf (ir_array, argv[2]);

    ListDtor (ir_array);
    TreeDtor (main_node);
}
