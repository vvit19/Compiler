#ifndef LIST_H
#define LIST_H

#include "config.h"

const int INITIAL_LIST_CAPACITY = 50;
const int FICT_ELEM      =  0;
const int PREV_FOR_FREE  = -1;

enum ListErrors
{
    NO_LIST_ERROR,
    LIST_IS_FULL,
    POS_BIGGER_THAN_CAPACITY,
    INVALID_NEW_CAPACITY,
    INVALID_POSITION_FOR_DELETE,
};

struct ListNode
{
    list_elem_t  value;
    int     prev;
    int     next;
};

struct List
{
    ListNode*  nodes;
    int    capacity;
    int    size;
    bool   linear;

    int    free;
    int    head;
    int    tail;
};

void        ListCtor      (List* list, int capacity);
ListErrors  InsertHead    (List* list, list_elem_t value);
ListErrors  InsertTail    (List* list, list_elem_t value);
ListErrors  InsertAfter   (List* list, list_elem_t value, int position);
ListErrors  InsertBefore  (List* list, list_elem_t value, int position);
ListErrors  ListDelete    (List* list, int position);
void        ListLinearise (List* list);
ListErrors  ListResize    (List* list, int new_capacity);
void        ListDtor      (List* list);

#endif // LIST_H
