#include <iostream>
#include <fstream>

#include "view.h"
#include "tui.h"
#include "human.h"
#include "game.h"
#include "ai.h"
#include <unistd.h>


int main() {
    View* v = View::get();
    Game* g = Game::get();

    Snake s;
    Snake s_ai_1;
    Snake s_ai_2;

    Ai ai_1(&s_ai_1, g);
    Ai ai_2(&s_ai_2, g);
    Human h(&s, g);

    g->add(&s);

    g->controls.push_back(&ai_1);
    g->add(&s_ai_1);

    g->controls.push_back(&ai_2);
    g->add(&s_ai_2);

    v->setModel(g);
    v->draw();
    v->run();

    delete v;
    return 0;
}
