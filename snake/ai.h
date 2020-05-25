#ifndef AI_H
#define AI_H


#include "control.h"


class Ai : public Control {
public:
    Ai(Snake*, Game*);

    ~Ai();

    Game* game_;

    void onMove();
};


#endif // AI_H
