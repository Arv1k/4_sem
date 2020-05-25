#ifndef CONTROL_H
#define CONTROL_H


#include "game.h"

class Control {
public:
    explicit Control(Snake *s) {
        snake = s;
    };

    Snake *snake;

    virtual void onMove() = 0;

    ~Control() {}
};


#endif // CONTROL_H
