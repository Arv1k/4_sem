#include <iostream>
#include <fstream>
#include <cstring>
#include "view.h"
#include "tui.h"

View * View::inst_ = nullptr;

View::~View() {
    inst_ = nullptr;
}

View * View::get() {
    if (inst_ != nullptr) {
        return inst_;
    }

    inst_ = new Tui;
    return inst_;
}

void View::setOnTimer(int time, Timeoutable timer) {
    std::pair<long, Timeoutable> res;

    res.first = time;
    res.second = timer;
    timer_.emplace_back(res);
}