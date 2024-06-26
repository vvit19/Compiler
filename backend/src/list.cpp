#include <cassert>
#include <cstdlib>
#include <cstdio>

#include "list.h"

static ListErrors VerifyFullList (List* list);

#ifndef NDEBUG
    #define FULL_LIST_VERIFY(list) \
        if ((VerifyFullList (list)) == LIST_IS_FULL) return LIST_IS_FULL;
#else
    #define FULFULL_LIST_VERIFY(list)
#endif

void ListCtor (List* list, int capacity)
{
    assert (list);

    list->nodes = (ListNode*) calloc (capacity, sizeof (ListNode));
    assert (list->nodes);

    list->capacity = capacity;

    for (int i = 1; i < capacity - 1; i++)
    {
        list->nodes[i].next  = i + 1;
        list->nodes[i].prev  = PREV_FOR_FREE;
        list->nodes[i].value = nullptr;
    }

    list->nodes[FICT_ELEM].next = FICT_ELEM;
    list->nodes[FICT_ELEM].prev = FICT_ELEM;

    list->nodes[capacity - 1].next  = FICT_ELEM;
    list->nodes[capacity - 1].prev  = PREV_FOR_FREE;
    list->nodes[capacity - 1].value = nullptr;

    list->head = FICT_ELEM;
    list->tail = FICT_ELEM;
    list->free = 1;
    list->linear = true;
}

ListErrors InsertHead (List* list, list_elem_t value)
{
    assert (list);

    FULL_LIST_VERIFY(list)

    int free_elem_id = list->free;
    list->free = list->nodes[list->free].next;

    list->nodes[list->head].prev = free_elem_id;

    list->nodes[free_elem_id].value = value;
    list->nodes[free_elem_id].next  = list->head;
    list->nodes[free_elem_id].prev  = FICT_ELEM;
    list->head = free_elem_id;

    if (list->size == 0)
    {
        list->tail = free_elem_id;
    }
    else
    {
        list->linear = false;
    }

    list->size++;

    return NO_LIST_ERROR;
}

ListErrors InsertTail (List* list, list_elem_t value)
{
    assert (list);

    FULL_LIST_VERIFY(list)

    int free_elem_id = list->free;
    list->free = list->nodes[list->free].next;

    list->nodes[list->tail].next = free_elem_id;

    list->nodes[free_elem_id].value = value;
    list->nodes[free_elem_id].prev  = list->tail;
    list->nodes[free_elem_id].next  = FICT_ELEM;
    list->tail = free_elem_id;

    if (list->size == 0)
    {
        list->head = free_elem_id;
    }

    list->size++;

    return NO_LIST_ERROR;
}

ListErrors InsertAfter (List* list, list_elem_t value, int position)
{
    assert (list);

    FULL_LIST_VERIFY(list)

    if (position == list->tail) return InsertTail (list, value);

    if (position >= list->capacity)
    {
        printf ("Position is bigger than capacity!\n");

        return POS_BIGGER_THAN_CAPACITY;
    }

    int free_elem_id = list->free;
    list->free = list->nodes[list->free].next;

    int after_position_id = list->nodes[position].next;
    list->nodes[position].next = free_elem_id;
    list->nodes[after_position_id].prev = free_elem_id;

    list->nodes[free_elem_id].prev  = position;
    list->nodes[free_elem_id].next  = after_position_id;
    list->nodes[free_elem_id].value = value;

    list->size++;

    list->linear = false;

    return NO_LIST_ERROR;
}

ListErrors InsertBefore (List* list, list_elem_t value, int position)
{
    assert (list);

    FULL_LIST_VERIFY(list)

    if (position == list->head) return InsertHead (list, value);

    if (position > list->capacity)
    {
        printf ("Position is bigger than capacity!\n");

        return POS_BIGGER_THAN_CAPACITY;
    }

    return InsertAfter (list, value, list->nodes[position].prev);
}

ListErrors ListDelete (List* list, int position)
{
    assert (list);

    if (position >= list->capacity)
    {
        printf ("Position is bigger than capacity!\n");

        return POS_BIGGER_THAN_CAPACITY;
    }

    if (list->size == 0)
    {
        printf ("No elements to delete\n");

        return INVALID_POSITION_FOR_DELETE;
    }

    list->nodes[position].value = nullptr;

    if (position == list->head)
    {
        if (list->head == list->tail) // One element in list
        {
            list->tail = FICT_ELEM;
            list->head = FICT_ELEM;
        }
        else
        {
            list->head = list->nodes[list->head].next;
            list->nodes[list->head].prev = FICT_ELEM;
            list->linear = false;
        }
    }
    else if (position == list->tail)
    {
        list->tail = list->nodes[list->tail].prev;
        list->nodes[list->tail].next = FICT_ELEM;
    }
    else
    {
        list->nodes[list->nodes[position].prev].next = list->nodes[position].next;
        list->nodes[list->nodes[position].next].prev = list->nodes[position].prev;
        list->linear = false;
    }

    int free_elem_id = list->free;

    list->free = position;
    list->nodes[list->free].prev = PREV_FOR_FREE;
    list->nodes[list->free].next = free_elem_id;

    list->size--;

    return NO_LIST_ERROR;
}

ListErrors ListResize (List* list, int new_capacity)
{
    assert (list);

    if ((new_capacity <= list->capacity) || (new_capacity <= 0))
    {
        printf ("Invalid new capacity!\n");

        return INVALID_NEW_CAPACITY;
    }

    list->nodes = (ListNode*) realloc (list->nodes, sizeof (ListNode) * new_capacity);

    list->capacity = new_capacity;

    ListLinearise (list);

    return NO_LIST_ERROR;
}

void ListLinearise (List* list)
{
    assert (list);

    ListNode* nodes_copy = (ListNode*) calloc (list->capacity, sizeof (ListNode));

    int cur_position = list->head;

    for (int i = 1; i < list->size + 1; i++)
    {
        nodes_copy[i].value = list->nodes[cur_position].value;
        nodes_copy[i].prev  = i - 1;
        nodes_copy[i].next  = i + 1;

        cur_position = list->nodes[cur_position].next;
    }

    free (list->nodes);
    list->nodes = nodes_copy;

    for (int i = list->size + 1; i < list->capacity; i++)
    {
        list->nodes[i].value = nullptr;
        list->nodes[i].prev  = PREV_FOR_FREE;
        list->nodes[i].next  = i + 1;
    }

    list->nodes[list->size].next = list->nodes[list->capacity - 1].next = 0;

    list->head   = 1;
    list->tail   = list->size;
    list->free   = list->size + 1;
    list->linear = true;
}

void ListDtor (List* list)
{
    assert (list);

    int capacity = list->capacity;
    ListNode* nodes = list->nodes;

    for (int i = 0; i < capacity; ++i)
        free (nodes[i].value);

    free (nodes);
    free (list);
}

static ListErrors VerifyFullList (List* list)
{
    assert (list);

    if (list->free == 0) ListResize (list, list->capacity * 2);

    return NO_LIST_ERROR;
}
