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
        } else if (ch == 'w' || ch == 'k') {
            state.moveCursor(nchess::model::MoveDirection::UP);
        } else if (ch == 'a' || ch == 'h') {
            state.moveCursor(nchess::model::MoveDirection::LEFT);
        } else if (ch == 's' || ch == 'j') {
            state.moveCursor(nchess::model::MoveDirection::DOWN);
        } else if (ch == 'd' || ch == 'l') {
            state.moveCursor(nchess::model::MoveDirection::RIGHT);
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