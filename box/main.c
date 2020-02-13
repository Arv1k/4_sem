#include "list/list.h"
#include "vector/vector.h"
#include <stdio.h>

int main() {
    box* list   = list_create();
    box* vector = vector_create();

    list->dump(list);

    // list little checking
    int arr[5];
    for (int i = 0; i < 5; i++) {
        arr[i] = 2*i;
        list->push(list, (void*) &arr[i]);
    }

    int hey = 90;
    list->insert(list, 3, &hey);
    list->dump(list);

    list->destroy(list);
    vector->destroy(vector);

    return 0;
}
