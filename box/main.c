#include "list/list.h"
#include "vector/vector.h"


int main() {
    struct box* list   = list_create();
    struct box* vector = vector_create();

    list->destroy(list);
    vector->destroy(vector);

    return 0;
}
