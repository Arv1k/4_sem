#include "ai.h"
#include "view.h"
#include <iostream>
#include <algorithm>
#include <random>

Ai::Ai(Snake * s, Game * g): Control(s), game_(g) {
    View::get()->setOnAi(this);
}

Ai::~Ai() {
    std::cout << "Ai dctr" << std::endl;
}

void Ai::onMove() {
    Dir dirs[] = {UP, DOWN, LEFT, RIGHT};

    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(dirs, dirs + 4, g);

    auto nearest_rabbit_coord = game_->near(snake->body_.front());
    auto cur_dist = nearest_rabbit_coord->distance(snake->body_.front());

    for (int i = 0; i < 4; i++) {
        snake->direction_ = dirs[i];

        char next_position = game_->checkForSnakes(snake->next());
        auto next_dist = nearest_rabbit_coord->distance(snake->next());
        if ((cur_dist > next_dist) &&
            (next_position == ' ' || next_position == 'r')) {
            break;
        }
    }
}
