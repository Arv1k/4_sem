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

    int        size;
    list_elem* head;
    list_elem* tail;
} list;


list_elem* list_find_item(list* this, int index) {
    list_elem* item = this->head;
    for (int i = 0; i < index; i++)
        item = item->next;

    return item;
}

int list_size(box* obj) {
    list* this = (list*) obj;

    return this->size;
}

void* list_item(box* obj, int index) {
    list* this = (list*) obj;

    if (index + 1 > this->size) {
        printf("The list size is less than %d!\n", index);

        return NULL;
    }

    list_elem* cur_item = list_find_item(this, index);

    return cur_item->data;
}

void list_swap(box* obj, int index_1, int index_2) {
    list* this = (list*) obj;
    if (index_1 > this->size)
        printf("The list size is less than %d!\n", index_1);

    if (index_2 > this->size)
        printf("The list size is less than %d!\n", index_2);

    list_elem* item_1 = list_find_item(this, index_1);
    list_elem* item_2 = list_find_item(this, index_2);

    void* tmp = item_1->data;

    item_1->data = item_2->data;
    item_2->data = tmp;
}

void list_push (box* obj, void* data) {
    list* this = (list*) obj;

    list_elem* item = (list_elem*) malloc(sizeof(list_elem));
    item->data = data;
    item->next = NULL;

    if (this->size > 0) {
        item->prev = this->tail;

        this->tail->next = item;
        this->tail = item;
    }

    else {
        item->prev = NULL;

        this->head = item;
        this->tail = item;
    }

    this->size++;
}

void list_pop (box* obj) {
    list* this = (list*) obj;

    if (this->size < 1) {
        printf("There isn't any items in this list!\n");

        return;
    }

    list_elem* cur_item = this->tail;

    if (this->size != 1) {
        this->tail = cur_item->prev;
        this->tail->next = NULL;
    }

    else {
        this->tail = NULL;
        this->head = NULL;
    }

    this->size--;

    free(cur_item);
}

void list_insert (box* obj, int index, void* data) {
    list* this = (list*) obj;

    if (index + 1 > this->size) {
        printf("The list size is less than %d!\n", index);

        return;
    }

    list_elem* cur_item = list_find_item(this, index);

    list_elem* item = (list_elem*) malloc(sizeof(list_elem));
    item->data = data;
    item->prev = cur_item;
    item->next = cur_item->next;

    if (cur_item->next)
        cur_item->next->prev = item;

    cur_item->next = item;

    this->size++;
}

void list_remove (box* obj, int index) {
    list* this = (list*) obj;

    if (index + 1 > this->size) {
        printf("The list size is less than %d!\n", index);

        return;
    }

    list_elem* cur_item = list_find_item(this, index);

    if (cur_item->prev)
        cur_item->prev->next = cur_item->next;

    else
        this->head = cur_item->next;

    if (cur_item->next)
        cur_item->next->prev = cur_item->prev;

    else
        this->tail = cur_item->prev;

    free(cur_item);

    this->size--;
}

void list_dump(box* obj) {
    list* this = (list*) obj;

    printf("\n");

    printf("+------ list dump ------+\n");
    printf("|    size: %d\n", this->size);

    list_elem* cur = this->head;
    printf("|    head: ");
    if (this->head)
        printf("%d [%p]\n", *((int*)cur->data), (void*) cur);

    else
        printf("? [NULL]\n");


    cur = this->tail;
    printf("|    tail: ");
    if (this->head)
        printf("%d [%p]\n", *((int*)cur->data), (void*) cur);

    else
        printf("? [NULL]\n");

    printf("|    items: ");
    cur = this->head;
    if (cur) {
        printf("\n");

        for (int i = 0; i < this->size; i++) {
            printf("|        %d: %d [%p]\n", i,
                                             *((int *) cur->data),
                                             (void *) cur);
            cur = cur->next;
        }
    }

    else
        printf("?\n");

    printf("\n");
}

void list_destroy(box* obj) {
    list* this = (list*) obj;

    int size = this->size;
    for (int i = 0; i < size; i++)
        list_pop(obj);

    free(this);
}

box* list_create() {
    list* this = (list*) malloc(sizeof(list));

    // list part
    this->size = 0;
    this->head = NULL;
    this->tail = NULL;

    // box part
    this->corobka.size = list_size;

    this->corobka.item = list_item;
    this->corobka.swap = list_swap;

    this->corobka.push = list_push;
    this->corobka.pop  = list_pop;

    this->corobka.insert = list_insert;
    this->corobka.remove = list_remove;

    this->corobka.dump = list_dump;

    this->corobka.destroy = list_destroy;

    return (box*) this;
}
