#ifndef BOX_BOX_H
#define BOX_BOX_H



typedef struct box {
    // variables
    int size;

    // functions
    void  (*destroy) (struct box* object);

    void* (*elem)    (struct box* objecr, int index);
    void  (*push)    (struct box* object, void* data);
    void  (*pop)     (struct box* object);
    void  (*insert)  (struct box* object, int index, void* data);
    void  (*remove)  (struct box* object, int index);
} box;



#endif // BOX_BOX_H
