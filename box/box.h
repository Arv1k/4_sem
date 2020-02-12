#ifndef BOX_BOX_H
#define BOX_BOX_H



typedef struct box {
    // variables
    int size;

    // functions
    void* (*item)    (struct box* object, int index);

    void  (*push)    (struct box* object, void* data);
    void  (*pop)     (struct box* object);

    void  (*insert)  (struct box* object, int index, void* data);
    void  (*remove)  (struct box* object, int index);

    void  (*destroy) (struct box* object);
} box;



#endif // BOX_BOX_H
