#include "vector.h"
#include <stdlib.h>
#include <stdio.h>



typedef struct vector {
    box    corobka;

    int    size;
    int    capacity;
    void** dataset;
} vector;


void vector_expand(vector* this) {
    if (this->dataset) {
        this->capacity *= 2;
    }

    else {
        this->size     = 0;
        this->capacity = 2;
    }

    this->dataset = (void**) realloc(this->dataset,
                                     this->capacity*sizeof(void*));
}

void vector_trim(vector* this) {
    this->capacity /= 2;
    this->dataset   = (void**) realloc(this->dataset,
                                       this->capacity*sizeof(void*));
}

int vector_size(box* obj) {
    vector* this = (vector*) obj;

    return this->size;
}

void* vector_item(box* obj, int index) {
    vector* this = (vector*) obj;

    if (index + 1 > this->size) {
        printf("The vector size is less than %d!\n", index);

        return NULL;
    }

    return this->dataset[index];
}

void vector_swap(box* obj, int index_1, int index_2) {
    vector* this = (vector*) obj;

    void* tmp = this->dataset[index_1];
    this->dataset[index_1] = this->dataset[index_2];
    this->dataset[index_2] = tmp;
}

void vector_push(box* obj, void* data) {
    vector* this = (vector*) obj;

    if (this->size == this->capacity)
        vector_expand(this);

    this->dataset[this->size++] = data;
}

void vector_pop(box* obj) {
    vector* this = (vector*) obj;

    if ((this->size - 1)*4 == this->capacity)
        vector_trim(this);

    this->dataset[--this->size] = NULL;
}

void vector_insert(box* obj, int index, void* data) {
    vector* this = (vector*) obj;

    if (index + 1 > this->size) {
        printf("The vector size is less than %d!\n", index);

        return;
    }

    if (this->size == this->capacity)
        vector_expand(this);

    for (int i = this->size; i > index; i--)
        this->dataset[i] = this->dataset[i - 1];

    this->dataset[index + 1] = data;
    this->size++;
}

void vector_remove (box* obj, int index) {
    vector* this = (vector*) obj;

    if (index + 1 > this->size) {
        printf("The vector size is less than %d!\n", index);

        return;
    }

    if ((this->size - 1)*4 == this->capacity)
        vector_trim(this);

    for (int i = index; i < this->size; i++) {
        this->dataset[i] = this->dataset[i + 1];
    }

    this->size--;
}

void vector_dump (box* obj) {
    vector* this = (vector*) obj;

    printf("\n");

    printf("+------ vector dump ------+\n");
    printf("|        size: %d\n", this->size);
    printf("|    capacity: %d\n", this->size);
    printf("|        items: ");

    if (this->dataset) {
        printf("\n");

        for (int i = 0; i < this->size; i++)
            printf("|            %d: %d\n", i, *((int*) this->dataset[i]));
    }

    else
        printf("?\n");

    printf("\n");
}


void vector_destroy(box* obj) {
    vector* this = (vector*) obj;

    this->size = 0;
    this->capacity = 0;
    free(this->dataset);

    free(this);
}

box* vector_create() {
    vector* this = (vector*) malloc(sizeof(vector));

    // vector part
    this->size     = 0;
    this->capacity = 0;
    this->dataset  = NULL;

    // box part
    this->corobka.size = vector_size;

    this->corobka.swap = vector_swap;
    this->corobka.item = vector_item;

    this->corobka.push = vector_push;
    this->corobka.pop  = vector_pop;

    this->corobka.insert = vector_insert;
    this->corobka.remove = vector_remove;

    this->corobka.dump = vector_dump;

    this->corobka.destroy = vector_destroy;

    return (box*) this;
}
