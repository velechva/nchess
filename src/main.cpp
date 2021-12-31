#include <algorithm>
#include <optional>

#include "ui.h"
#include "validation/validate.h"
#include "model/state.h"

int main() {
    nchess::ui::initialize();
    nchess::model::State state;
    nchess::ui::refresh(state);

    char ch = ' ';

    while (ch != 'e') {
        ch = nchess::ui::getChar();

        if (ch == 'e') {
            nchess::ui::exit();
        } else if (ch == 'w') {
            state.cursor.first = std::max(0, state.cursor.first - 1);
        } else if (ch == 'a') {
            state.cursor.second = std::max(0, state.cursor.second - 1);
        } else if (ch == 's') {
            state.cursor.first = std::min(7, state.cursor.first + 1);
        } else if (ch == 'd') {
            state.cursor.second = std::min(7, state.cursor.second + 1);
        } else if (ch == 'm') {
            if (!state.currentMove) {
                if (!nchess::validation::validateMoveBegin(state, state.cursor)) {
                    nchess::ui::printError(state, "Invalid move");
                } else {
                    state.currentMove = state.cursor;
                }
            } else {
                if (!nchess::validation::validateMoveEnd(state, state.currentMove.value(), state.cursor, true)) {
                    nchess::ui::printError(state, "Invalid move");
                } else {
                    state.movePiece(state.currentMove.value(), state.cursor);
                    state.isWhiteTurn = !state.isWhiteTurn;

                    nchess::model::State newState(state);
                    if (nchess::validation::inCheckMate(newState)) {
                        nchess::ui::printWinner(!state.isWhiteTurn);
                        nchess::ui::getChar();
                        nchess::ui::exit();
                    }
                }
                state.currentMove = std::nullopt;
            }
        }
        nchess::ui::refresh(state);
    }
    nchess::ui::exit();
}