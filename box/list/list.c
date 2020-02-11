#include "list.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


typedef struct {
    box   corobka;
    char* name;
} list;


void list_destroy(box* obj) {
    list* this = (list*) obj;

    printf("%s ", this->name);
    free(this->name);

    free(this);

    printf("ubit\n");
}

struct box* list_create() {
    list* this = (list*) malloc(sizeof(list));

    this->corobka.destroy = list_destroy;
    this->name = strdup("Ya est' LIST");

    return (box*) this;
}
