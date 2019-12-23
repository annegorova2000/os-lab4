#ifndef __list_h__
#define __list_h__

#define STACK_INIT_SIZE 2
#define STACK_MULTIPLIER 2

typedef struct {
    int* data;
    int top;
    int size;
} List;

List* list_create();  // create instance of list
void list_delete(List* st);   // delets list
void list_resize(List* st);   // adjast size = curr_size * STACK_MULRIPLIER
void list_push(List* st, int val);    // insert item
int list_peek(List* st);  // get last item
void list_pop(List* st);  // removes last item
int list_size(List* st);  // size of list
void list_print(List* st);    // outputs list's elements
void list_implode(List* st);  // if we know that list won't change ( free some memorv )
int list_is_empty(List* st);  // return 1 if empty
int list_top_index(List* st); // return index of top element
void list_conc(List* bot, List* top); // add items to bot list and delets top

List* list_quick_sort(List* st);   // hoar

#endif
