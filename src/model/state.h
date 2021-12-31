#ifndef NCHESS_STATE_H
#define NCHESS_STATE_H

#include <vector>
#include <iostream>
#include <stdlib.h>

#include "piece.h"
#include "types.h"

namespace nchess::model {
    typedef std::vector<std::vector<Piece>> Board;

    struct State {
    public:
        Board board;
        bool isWhiteTurn;
        Position cursor;
        std::optional<Position> currentMove;

        Board initialBoardState();

        State() :
                board(initialBoardState()),
                isWhiteTurn(true),
                cursor{0, 0},
                currentMove(std::nullopt) {}

        State(const State &other) :
                board(Board(other.board)),
                isWhiteTurn(other.isWhiteTurn),
                cursor(Position(other.cursor)),
                currentMove(std::optional(other.currentMove)) {}

        void movePiece(const Position &begin, const Position &end);

        [[nodiscard]] const Piece &pieceAt(const Position &p) const {
            return board[p.first][p.second];
        }
    };
}

#endif