#include "list.h"
#include <stdio.h>
#include <stdlib.h>

List* list_create(){
  List* st = (List *)malloc(sizeof(List));

  st->data = (int *)malloc(sizeof(int) * LIST_INIT_SIZE);
  st->size = LIST_INIT_SIZE;
  st->top  = -1;

  return(st);
};

void list_push(List* st, int val){
  if (st->top == st->size - 1) {
     list_resize(st);
  }

  st->top += 1;
  st->data[st->top] = val;
}

int list_peek(List* st) {

  if (list_is_empty(st)) {
    fprintf(stderr, "list empty (list peek)\n");
    exit(2);
  }
  return(st->data[st->top]);
}

int list_top_index(List* st) {
    return(st->top);
}

void list_pop(List* st) {
  if (st->top == -1) {
    fprintf(stderr, "list empty (list pop)\n");
    exit(2);
  }
  st->top--;
}

void list_resize(List* st) {
  if (!st->size) {
    st->size = LIST_INIT_SIZE;
  } else {
    st->size *= LIST_MULTIPLIER;
  }
  st->data = (int *)realloc(st->data, sizeof(int) * st->size);
}

int list_is_empty(List* st) {
  return(st->top == -1);
}

void list_print(List* st) {
  for (int i = st->top; i >= 0; i--) {
    fprintf(stdout, "%d ", st->data[i]);
  } fprintf(stdout, "\n");
}



int list_size(List* st) {
  return (st->size);
}

void list_delete(List* st) {
  if (!list_is_empty(st)) return;
  st->top = -1;
  st->size = 0;
  free(st->data);
  free(st);
}

void list_implode(List* st) {
  st->size = st->top + 1;
  st->data = (int *)realloc(st->data, sizeof(int) * st->size);
}
