#include <iostream>
#include <climits>
#include "game.h"
#include "view.h"
#include "tui.h"
#include <random>


int Coord::distance(const Coord & c1/*, const Coord & c2*/) const {
    int res = abs(c1.first - this->first) + abs(c1.second - this->second);
    return res;
}

optional<Coord> Game::near(const Coord &c) const { /// 4 optional
    int min = INT_MAX;
    optional<Coord> closest_rabbit;

    for (auto i : rabbits) {
        if (i.distance(c) < min) {
            min = i.distance(c);
            fout << "min" << min << std::endl;
            closest_rabbit = i;
        }
    }

    fout << "closest_rabbit:" << closest_rabbit->first << " "
         << closest_rabbit->second << std::endl;

    return closest_rabbit;
}

Game::Game() {
    View* v = View::get();
    v->setOnTimer(500, std::bind(&Game::move, this));

    for (int i = 0; i < RABBITS_AMOUNT; ++i) {
        createRabbit();
        View::get()->timer_.pop_back();
    }

    v->setOnTimer(SNAKE_TIME, std::bind(&Game::move, this));
    v->setOnTimer(RABBIT_TIME, std::bind(&Game::move, this));
}

Snake::Snake() {
    alive_       = true;
    direction_   = RIGHT;
    Coord start  = Game::get()->getFreeCoord();
    for (int i = 0; i < 4; i++) {
        body_.emplace_back(Coord(start.first, start.second));
    }
}

Snake::Snake(const Snake & s):
    direction_   (s.direction_),
    body_        (s.body_),
    alive_       (s.alive_) {}

Game* Game::game_inst_;

Game* Game::get() {
    if (game_inst_ != nullptr) {
        return Game::game_inst_;
    }

    Game::game_inst_ = new Game;

    return Game::game_inst_;
}

Coord Game::getFreeCoord() const {
    Coord res;

    while (1) {
        res.first  = rand() % View::get()->view_x_;
        res.second = rand() % View::get()->view_y_;

        if (res.first  == 0 || res.first  == 1) res.first  += 5;
        if (res.second == 0 || res.second == 1) res.second += 5;

        if (res.first  == View::get()->view_x_ - 1) res.first  -= 5;
        if (res.second == View::get()->view_y_ - 1) res.second -= 5;

        if (isFree(res)) {
            return res;
        }
    }
}

bool Game::isFree(const Coord c) const {
    for (const auto i : snakes) {
        for (const auto &b : i->body_) {
            if (c == b) {
                return false;
            }
        }
    }

    for(const Coord &i : rabbits) {
        if (i == c) {
            return false;
        }
    }

    if(c.first  <= 1)   return false;
    if(c.second <= 1)   return false;

    if(c.first  >= View::get()->view_x_ - 1)   return false;
    if(c.second >= View::get()->view_y_ - 1)   return false;

    return true;
}

void Game::paint(SnakePainter snake_painter, RabbitPainter rabbit_painter) {
    bool head;
    for (const auto s: snakes) {
        head = true;
        for (const auto &b: s->body_) {
            snake_painter(b, head ? s->direction_ : NO);
            head = false;
        }
    }

    for (const auto &r: rabbits) {
        rabbit_painter(r);
    }
}

void Game::add(Snake * p) {
    snakes.push_back(p);
}

Snake& Snake::operator=(Snake const &s) {
    body_       = s.body_;
    direction_  = s.direction_;
    return      *this;
}

void Game::move() {
    bool game_over = true;

    View *v = View::get();

    for (const auto &s : snakes)
        if (s->alive_) {
            game_over = false;
            break;
        }

    for (auto s : snakes)
        if (s->alive_)
            s->move();

    v->draw();
    v->setOnTimer(SNAKE_TIME, std::bind(&Game::move, this));

    for (auto c : controls) {
        c->onMove();
    }
}

Coord Snake::next() {
    auto a = body_.front();

    switch (direction_) {
        case UP:
            a.first--;
            break;
        case DOWN:
            a.first++;
            break;
        case RIGHT:
            a.second++;
            break;
        case LEFT:
            a.second--;
            break;
    }

    return a;
}

void Snake::move() {

    auto a = next();

    switch (Game::get()->checkForSnakes(a)) {
        case ' ':
            body_.push_front(a);
            body_.pop_back();
            break;

        case 's':
            alive_ = false;
            fout << "GAME OVER: snake" << std::endl;
            break;

        case 'b':
            alive_ = false;
            fout << "GAME OVER: boundaries" << std::endl;
            break;

        case 'r':
            Game::get()->killRabbit(a);
            body_.push_front(a);
            break;

        default:
            break;
    }
}

void Game::killRabbit(const Coord c) {
    for (const auto &r : rabbits) {
        if (c == r) {
            rabbits.remove(r);
            break;
        }
    }
}

char Game::checkForSnakes(const Coord c) const {
    for (const auto i : snakes) {
        for (const auto &b : i->body_) {
            if (c == b) {
                return 's';
            }
        }
    }

    for (const auto &r : rabbits) {
        if (c == r) {
            return 'r';
        }
    }

    if (c.first <= 1 || c.second <= 1 || c.first >= View::get()->view_x_ ||
        c.second >= View::get()->view_y_)
        return 'b';

    return ' ';
}

void Game::createRabbit() {
    Coord res;
    srand(1);

    View::get()->setOnTimer(8, std::bind(&Game::createRabbit, this));

    while (1) {
        res.first = rand() % View::get()->view_x_;
        res.second = rand() % View::get()->view_y_;

        if (res.first == 0) res.first++;
        if (res.second == 0) res.second++;

        if (res.first == View::get()->view_x_ - 1) res.first--;
        if (res.second == View::get()->view_y_ - 1) res.second--;

        if (isFree(res)) {
            rabbits.push_back(res);
            return;
        }
    }
}
