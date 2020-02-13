#ifndef BOX_BOX_H
#define BOX_BOX_H



typedef struct box {
    // functions
    int   (*size)    (struct box* object);

    void* (*item)    (struct box* object, int index);
    void  (*swap)    (struct box* object, int index_1, int index_2);

    void  (*push)    (struct box* object, void* data);
    void  (*pop)     (struct box* object);

    void  (*insert)  (struct box* object, int index, void* data);
    void  (*remove)  (struct box* object, int index);

    void  (*dump)    (struct box* object);

    void  (*destroy) (struct box* object);
} box;



#endif // BOX_BOX_H
