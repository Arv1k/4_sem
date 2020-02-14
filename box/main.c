#include "list/list.h"
#include "vector/vector.h"
#include <stdio.h>
#include <stdlib.h>


int comparator_int(void* data_1, void* data_2) {
    int num_1 = *((int*) data_1);
    int num_2 = *((int*) data_2);

    if (num_1 > num_2)
        return -1;

    else if (num_1 == num_2)
        return 0;

    else
        return 1;
}

void sort(box* obj) {
    int size = obj->size(obj);

    for (int i = 0; i < size - 1;     i++)
    for (int j = 0; j < size - i - 1; j++)
        if (comparator_int(obj->item(obj, j), obj->item(obj, j + 1)) > 0)
            obj->swap(obj, j, j + 1);
}

int main() {
    box* list   = list_create();
    box* vector = vector_create();

    int arr[5];
    for (int i = 0; i < 5; i++) {
        arr[i] = 2*i;
        list->push(list, (void*) &arr[i]);
    }

    int hey = 9;
    list->insert(list, 3, &hey);
    list->dump(list);

    sort(list);

    list->dump(list);

    int a[] = {1, 7, 3, 5, 6, 8};
    for (int i = 0; i < 5; i++)
        vector->push(vector, &a[i]);

    vector->insert(vector, 1, &a[5]);
    vector->dump(vector);

    sort(vector);

    vector->dump(vector);

    list->destroy(list);
    vector->destroy(vector);

    return 0;
}
