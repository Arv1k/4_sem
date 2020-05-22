#include <iostream>
#include <fstream>

#include "game.h"
#include "human.h"
#include "view.h"
#include "tui.h"

Human::Human(Snake * s, Game *g): Control(s), game(g) {
    View::get()->setOnKey(this);
}

Human::~Human() {
    std::cout << "Human dctr called\n";
}

void Human::onkey(int key) {
    //printf("key pressed = %d ", n);
    View *v = View::get();
    switch (key) {
        case 'q':
            delete v;
            exit(EXIT_SUCCESS);
        case 'd':
            if (snake->direction_ == LEFT) break;
            snake->direction_ = RIGHT;
            break;
        case 'a':
            if (snake->direction_ == RIGHT) break;
            snake->direction_ = LEFT;
            break;
        case 'w':
            if (snake->direction_ == DOWN) break;
            snake->direction_ = UP;
            break;
        case 's':
            if (snake->direction_ == UP) break;
            snake->direction_ = DOWN;
            break;
    }
    //game->move();
    //View::get()->draw();
}