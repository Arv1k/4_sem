#include "vector.h"
#include <stdlib.h>
#include <stdio.h>



typedef struct vector {
    box    corobka;
    int    capacity;
    void** dataset;
} vector;


void vector_destroy(box* obj) {
    vector* this = (vector*) obj;



    free(this);
}

box* vector_create() {
    vector* this = (vector*) malloc(sizeof(vector));

    this->corobka.destroy = vector_destroy;

    return (box*) this;
}
