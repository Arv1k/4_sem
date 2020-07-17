#ifndef TUI_H
#define TUI_H


#include <termios.h>
#include <fstream>
#include "view.h"


static std::ofstream fout("logFile.txt");

class Tui : public View {
private:
    struct termios old_;

public:
    Tui();
    ~Tui();

    void draw();
    void getWinSize();

    void drawLineY(int, int, int);
    void drawLineX(int, int, int);

    void gotoxy(int, int); //moves to the certain position
    void run();

    void clearScreen();

    void snakePainter(Coord, Dir);
    void rabbitPainter(const Coord);
};


#endif // TUI_H