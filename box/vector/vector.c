#include "vector.h"
#include <stdlib.h>
#include <stdio.h>



typedef struct vector {
    box    corobka;
    int    capacity;
    void** dataset;
} vector;


void* vector_item (box* obj, int index) {

}

void vector_push (box* object, void* data) {

}

void vector_pop (box* object) {

}

void vector_insert (box* object, int index, void* data) {

}

void vector_remove (box* object, int index) {

}

void vector_destroy(box* obj) {
    vector* this = (vector*) obj;

    free(this);
}

box* vector_create() {
    vector* this = (vector*) malloc(sizeof(vector));

    // vector part
    this->capacity = 0;
    this->dataset = NULL;

    // box part
    this->corobka.size = 0;

    this->corobka.item = vector_item;

    this->corobka.push = vector_push;
    this->corobka.pop  = vector_pop;

    this->corobka.insert = vector_insert;
    this->corobka.remove = vector_remove;

    this->corobka.destroy = vector_destroy;

    return (box*) this;
}
