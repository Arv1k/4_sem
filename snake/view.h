#ifndef VIEW_H
#define VIEW_H

#include "game.h"
#include "keypressable.h"
#include "ai.h"

using Timeoutable = std::function<void()>;

class View {
public:
    void virtual draw() = 0;

    void virtual run() = 0;

    virtual             ~View() = 0;

    static View *inst_;

    //static View *       get         (const char * type = nullptr);
    static View *get();

    virtual void getWinSize() = 0;

    void setOnKey(KeyPressable *key) { onkey_delegate_ = key; };
    KeyPressable *onkey_delegate_;
    Ai *ai_delegate_;

    void setOnAi(Ai *ai) { ai_delegate_ = ai; };

    Game *game_;

    void setModel(Game *g) { game_ = g; };

    void virtual snakePainter(Coord c, Dir d) = 0;

    void virtual rabbitPainter(Coord c) = 0;

    void setOnTimer(int time, Timeoutable timer);

    std::list<std::pair<int, Timeoutable>> timer_;

    //TODO:find out how to get size of window from Tui
    int view_x_, view_y_;
//private:

};

#endif