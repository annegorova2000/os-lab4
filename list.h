#ifndef __list_h__
#define __list_h__

#define LIST_INIT_SIZE 2
#define LIST_MULTIPLIER 2

typedef struct {
    int* data;
    int top;
    int size;
} List;

List* list_create();  
void list_delete(List* st);  
void list_resize(List* st);   
void list_push(List* st, int val);
int list_peek(List* st);
void list_pop(List* st); 
int list_size(List* st);  
void list_print(List* st);    
void list_implode(List* st); 
int list_is_empty(List* st);  
int list_top_index(List* st); 
void list_conc(List* bot, List* top); 

List* list_quick_sort(List* st);  

#endif
