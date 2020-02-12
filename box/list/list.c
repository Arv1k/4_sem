#include "list.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>



typedef struct list_elem {
    void* data;

    struct list_elem* next;
    struct list_elem* prev;
} list_elem;

typedef struct list {
    box corobka;

    list_elem* head;
    list_elem* tail;
} list;


void* list_item (box* obj, int index) {
    if (index + 1 > obj->size) {
        printf("The list size is less than %d!\n", index);

        return NULL;
    }

    list* this = (list*) obj;

    list_elem* cur_item = this->head;
    for (int i = 0; i < index; i++)
        cur_item = cur_item->next;

    return cur_item->data;
}

void list_push (box* obj, void* data) {
    printf("Hey\n");
    list* this = (list*) obj;

    printf("Hey\n");
    list_elem* item = (list_elem*) malloc(sizeof(list_elem));
    printf("Hey\n");
    item->data = data;
    item->prev = this->tail;
    item->next = NULL;

    printf("Hey\n");
    this->tail->next = item;
    this->tail       = item;
    this->corobka.size++;
    printf("Hey\n");
}

void list_pop (box* obj) {
    list* this = (list*) obj;

    list_elem* cur_item = this->tail;

    this->tail = cur_item->prev;
    this->tail->next = NULL;
    this->corobka.size--;

    free(cur_item);
}

void list_insert (box* obj, int index, void* data) {
    if (index + 1 > obj->size) {
        printf("The list size is less than %d!\n", index);

        return;
    }

    list* this = (list*) obj;

    list_elem* cur_item = this->head;
    for (int i = 0; i < index; i++)
        cur_item = cur_item->next;

    list_elem* item = (list_elem*) malloc(sizeof(list_elem));
    item->data = data;
    item->prev = cur_item;
    item->next = cur_item->next;

    cur_item->next->prev = item;
    cur_item->next = item;

    this->corobka.size++;
}

void list_remove (box* obj, int index) {
    if (index + 1 > obj->size) {
        printf("The list size is less than %d!\n", index);

        return;
    }

    list* this = (list*) obj;

    list_elem* cur_item = this->head;
    for (int i = 0; i < index; i++)
        cur_item = cur_item->next;

    cur_item->prev->next = cur_item->next;
    cur_item->next->prev = cur_item->prev;

    free(cur_item);

    this->corobka.size--;
}

void list_destroy(box* obj) {
    list* this = (list*) obj;

    free(this);
}

box* list_create() {
    list* this = (list*) malloc(sizeof(list));

    // list part
    this->head = NULL;
    this->tail = NULL;

    // box part
    this->corobka.size = 0;

    this->corobka.item = list_item;

    this->corobka.push = list_push;
    this->corobka.pop  = list_pop;

    this->corobka.insert = list_insert;
    this->corobka.remove = list_remove;

    this->corobka.destroy = list_destroy;

    return (box*) this;
}
