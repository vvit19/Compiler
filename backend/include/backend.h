#ifndef BACKEND_H
#define BACKEND_H

#include "lang_tree.h"
#include "reader.h"
#include "list.h"

List* AstToIr (Node* main_node);
void  TranslateToAsm  (List* ir_array, const char* filename);
void  TranslateToElf  (List* ir_array, const char* filename);

#endif
