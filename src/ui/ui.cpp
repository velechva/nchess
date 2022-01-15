#include <ncurses.h>

#include "ui.h"

void setupColors() {
    if (has_colors() == FALSE) {
        printf("Terminal does not support colors.");
        exit(1);
    }

    start_color();
    init_pair(1, COLOR_WHITE, COLOR_BLACK);
    init_pair(2, COLOR_BLUE, COLOR_BLACK);
}

void nchess::ui::initialize() {
    initscr();
    raw();
    noecho();
    setupColors();
}

void nchess::ui::printControls() {
    printw("w: up, a: left, s: down, d: right, m: move, e: exit\n");
}

void nchess::ui::printError(const model::State &state, const char *str) {
    printMessage(str);
    move(state.cursor.first, state.cursor.second);
    getChar();
}

void nchess::ui::refresh(const model::State &state) {
    clear();
    printBoard(state);
    printControls();
    ::refresh();
    move(state.cursor.first, state.cursor.second);
}

void nchess::ui::exit() {
    clear();
    ::refresh();
    endwin();
    ::exit(0);
}

void nchess::ui::printWinner(const bool winnerIsWhite) {
    std::string winnerStr = winnerIsWhite ? "White" : "Black";
    winnerStr += " wins. Press any key to exit";
    printMessage(winnerStr.c_str());
    ::refresh();
}

void nchess::ui::printMessage(const char *str) {
    move(12, 0);
    printw(str);
    ::refresh();
}

void nchess::ui::printBoard(const model::State &state) {
    auto board = state.board;

    for (auto &i: board) {
        for (auto &j: i) {
            if (j.kind == model::NONE) {
                printw("-");
            } else if (j.isWhite) {
                attron(COLOR_PAIR(1));
                printw(j.toString());
                attroff(COLOR_PAIR(1));
            } else {
                attron(COLOR_PAIR(2));
                printw(j.toString());
                attroff(COLOR_PAIR(2));
            }
        }
        printw("\n");
    }
    printw("\n\n");
}

char nchess::ui::getChar() {
    return getch();
}