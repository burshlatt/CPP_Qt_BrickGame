#include <stdio.h>
#include <ncurses.h>
#include <stdbool.h>

#include "console_view.h"

static const char menu[18][20] = {
    "Next figure:\t",
    "",
    "",
    "",
    "",
    "\t\t",
    "\t\t",
    "\t\t",
    "\t\t",
    "\t\t",
    "\t\t",
    "1 - Start\t",
    "2 - Pause/Resume",
    "< - Move left\t",
    "> - Move right\t",
    "V - Move down\t",
    "r - Rotate\t",
    "q - Exit\t",
};

void DrawField(GameInfo_t info) {
    clear();

    printw("+----------+----------------------------+\n");

    for (int i = 0; i < FIELD_ROWS; ++i) {
        for (int j = 0; j < FIELD_COLS; ++j) {
            if (j == 0)
                printw("|");

            printw("%c", info.field[i][j]);
        }

        if (i > 17)
            printw("|\t\t\t\t|\n");
        else if (i > 0 && i < 5)
            printw("|\t%s\t\t|\n", info.next_figure.figure[i - 1]);
        else if (i == 6)
            printw("|\tHigh score: %d\t|\n", info.high_score);
        else if (i == 7)
            printw("|\tScore: %d\t\t|\n", info.score);
        else if (i == 9)
            printw("|\tLevel: %d\t\t|\n", info.level);
        else
            printw("|\t%s\t|\n", menu[i]);
    }

    printw("+----------+----------------------------+\n");

    refresh();
}

void ShowView() {
    initscr();
    cbreak();
    noecho();
    curs_set(0);
    keypad(stdscr, TRUE);

    bool exit = false;

    GameInfo_t info = InitializeInfo();

    while (true) {
        timeout(info.speed);

        int key = getch();

        switch (key) {
            case '1':
                UserInput(kStart);
                break;
            case '2':
                UserInput(kPause);
                break;
            case KEY_DOWN:
                UserInput(kDown);
                break;
            case KEY_LEFT:
                UserInput(kLeft);
                break;
            case KEY_RIGHT:
                UserInput(kRight);
                break;
            case 'r':
                UserInput(kAction);
                break;
            case 'q':
                UserInput(kTerminate);
                exit = true;
                break;
        }

        if (exit)
            break;

        info = UpdateCurrentState();
        DrawField(info);
    }

    endwin();
}
