#ifndef NCHESS_UI_H
#define NCHESS_UI_H

#include "../model/state.h"

namespace nchess::ui {
    /* Application lifecycle */
    void initialize();
    void refresh(const nchess::model::State &state);
    void exit();

    /* Printing */
    void printControls();
    void printPiece(const model::Piece& piece);
    void printWinner(const bool winnerIsWhite);
    void printMessage(const char *str);
    void printError(const model::State &state, const char *str);
    void printBoard(const model::State &state);

    char getChar();
}

#endif