#include "list.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>



typedef struct list_elem {
    void*             data;
    struct list_elem* right;
    struct list_elem* left;
} list_elem;

typedef struct list {
    box        corobka;
    list_elem* head;
    list_elem* tail;
} list;


void list_destroy(box* obj) {
    list* this = (list*) obj;



    free(this);
}

box* list_create() {
    list* this = (list*) malloc(sizeof(list));

    this->corobka.destroy = list_destroy;

    return (box*) this;
}
