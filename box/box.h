#ifndef BOX_BOX_H
#define BOX_BOX_H



typedef struct box {
    void (*destroy)(struct box* object);
} box;



#endif // BOX_BOX_H
