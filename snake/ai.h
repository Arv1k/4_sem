#ifndef WFCPP_AI_H
#define WFCPP_AI_H

#include "control.h"

class Ai : public Control {
public:
    Ai(Snake *, Game *);

    ~Ai();

    Game *game_;

    void onMove();
};

#endif //WFCPP_AI_H
