#include <optional>

#include "ui.h"
#include "validation/validate.h"
#include "model/state.h"

using namespace nchess;

void handleCheckmateCondition(const model::State& state) {
    model::State newState(state);
    if (validation::inCheckMate(newState)) {
        ui::printWinner(!state.isWhiteTurn);
        ui::getChar();
        ui::exit();
    }
}

void handleMove(model::State& state) {
    if (!state.currentMove) {
        if (!validation::validateMoveBegin(state, state.cursor)) {
            ui::printError(state, "Invalid move");
        } else {
            state.currentMove = state.cursor;
        }
    } else {
        if (validation::validateCastle(state, state.currentMove.value(), state.cursor)) {
            state.swapPieces(state.currentMove.value(), state.cursor);
        }
        else if(validation::validateMoveEnd(state, state.currentMove.value(), state.cursor, true)) {
            state.movePiece(state.currentMove.value(), state.cursor);
        }
        else {
            ui::printError(state, "Invalid move");
            state.currentMove = std::nullopt;
            return;
        }

        state.changePlayer();
        handleCheckmateCondition(state);

        state.currentMove = std::nullopt;
    }
}

int main() {
    model::State state;

    ui::initialize();
    ui::refresh(state);

    char ch = ' ';

    while (ch != 'e') {
        ch = ui::getChar();

        if (ch == 'e') {
            ui::exit();
        } else if (ch == 'w' || ch == 'k') {
            state.moveCursor(model::DELTA_UP);
        } else if (ch == 'a' || ch == 'h') {
            state.moveCursor(model::DELTA_LEFT);
        } else if (ch == 's' || ch == 'j') {
            state.moveCursor(model::DELTA_DOWN);
        } else if (ch == 'd' || ch == 'l') {
            state.moveCursor(model::DELTA_RIGHT);
        } else if (ch == 'm') {
            handleMove(state);
        }
        ui::refresh(state);
    }
    ui::exit();
}