#include "list/list.h"
#include "vector/vector.h"
#include <stdio.h>

int main() {
    box* list   = list_create();
    box* vector = vector_create();

    int x = 5;
    list->push(list, (void*) &x);


    printf("What\n");

    list->destroy(list);

    printf("a\n");

    vector->destroy(vector);

    printf("FUCK?\n");

    return 0;
}
