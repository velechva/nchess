#include <ncurses.h>

#include "ui.h"
#include "colors.h"

void nchess::ui::initialize() {
    initscr();
    raw();
    noecho();
    setupColors();
}

void nchess::ui::printControls() {
    print("w: up, a: left, s: down, d: right, m: move, e: exit\n");
}

void nchess::ui::printCurrentPlayer(const model::State &state) {
    if (state.isWhiteTurn) {
        print("White turn\n", CYAN);
    }
    else {
        print("Black turn\n", BLUE);
    }
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
    printCurrentPlayer(state);
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
    print(str);
    ::refresh();
}

void nchess::ui::printPiece(const nchess::model::Piece& piece) {
    if (piece.kind == model::NONE) {
        nchess::ui::print("-");
    } else if (piece.isWhite) {
        print(piece.toString(), CYAN);
    } else {
        print(piece.toString(), BLUE);
    }
}

void nchess::ui::printBoard(const model::State &state) {
    auto board = state.board;

    for (auto &rank: board) {
        for (auto &piece: rank) {
            printPiece(piece);
        }
        print("\n");
    }
    print("\n\n");
}

char nchess::ui::getChar() {
    return getch();
}