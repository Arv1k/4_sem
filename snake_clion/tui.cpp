#include <iostream>
#include <termios.h>
#include <sys/ioctl.h>
#include <csignal>
#include <unistd.h>
#include <functional>
#include <poll.h>


using namespace std::placeholders;

#include "tui.h"

void Tui::drawLineX(int start, int length, int y) {
    for (int i = start; i < start + length; ++i) {
        gotoxy(i, y);
        printf("*");
    }
}

void Tui::drawLineY(int start, int length, int x) {
    for (int i = start; i < start + length; ++i) {
        gotoxy(x, i);
        printf("*");
    }
}

void Tui::draw() {
    clearScreen();
    getWinSize();

    drawLineX(0, view_x_, 0);
    drawLineY(0, view_y_, 0);
    drawLineX(0, view_x_, view_y_);
    drawLineY(0, view_y_, view_x_);

    game_->paint(std::bind(&View::snakePainter, this, _1, _2),
                 std::bind(&View::rabbitPainter, this, _1));

    fflush(stdout);
}

void Tui::getWinSize() {
    clearScreen();

    struct winsize ws;
    ioctl(0, TIOCGWINSZ, &ws);

    view_x_ = ws.ws_row;
    view_y_ = ws.ws_col;
}

void onwinch(int x) {
    View::get()->draw();
}

Tui::Tui() {
    clearScreen();

    getWinSize();
    //setbuf(stdout, NULL);

    struct sigaction sa = {0}; /// 3
    sa.sa_handler = onwinch;
    sa.sa_flags = SA_RESTART;
    sigaction(SIGWINCH, &sa, nullptr);

    struct termios cur_mode;
    tcgetattr(0, &cur_mode);
    old_ = cur_mode;
    cfmakeraw(&cur_mode);
    tcsetattr(0, TCSAFLUSH, &cur_mode);
}


void Tui::clearScreen() {
    printf("\ec");
}

Tui::~Tui() {
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &old_);

    clearScreen();
    printf("Goodbye\n");
}

void Tui::run() {
    char cmd;

    nfds_t nfds = 1;
    int fds_ready = -1;

    pollfd* poll_stdin_master = new struct pollfd[nfds];
    pollfd* poll_stdin_set = new struct pollfd[nfds];

    poll_stdin_master[0].fd = STDIN_FILENO;
    poll_stdin_master[0].events = POLLIN;


    draw();
    while (1) {
        for (unsigned int i = 0; i < nfds; ++i)
            poll_stdin_set[i] = poll_stdin_master[i];

        fds_ready = poll(poll_stdin_set, nfds, timer_.front().first);
        if (fds_ready < 0) {
            fout << "ERROR: poll " << __PRETTY_FUNCTION__ << errno << std::endl;
            break;
        } else if (fds_ready == 0) {
            std::pair<long, Timeoutable> a = timer_.front();
            //timer_.pop_front();
            a.second();
            continue;
        }

        for (unsigned int i = 0; i < nfds; ++i) {
            if (poll_stdin_set[i].revents == POLLIN) {
                if (read(poll_stdin_set[i].fd, &cmd, 1) == -1) {
                    fout << "ERROR: reading from pool set"
                         << __PRETTY_FUNCTION__ << std::endl;
                    break;
                }

                if (onkey_delegate_) onkey_delegate_->onkey(cmd);
            }
        }

    }

    delete[] poll_stdin_master;
    delete[] poll_stdin_set;
}

void Tui::gotoxy(int x, int y) {
    printf("\e[%d;%dH", x, y);
}

void Tui::snakePainter(Coord c, Dir d) {
    gotoxy(c.first, c.second);
    putchar("^v><#"[d]);
}

void Tui::rabbitPainter(const Coord c) {
    gotoxy(c.first, c.second);
    putchar('@');
}
