#ifndef NCHESS_STATE_H
#define NCHESS_STATE_H

#include <vector>
#include <iostream>
#include <stdlib.h>

#include "piece.h"
#include "types.h"

namespace nchess::model {
    typedef std::vector<std::vector<Piece>> Board;

    enum MoveDirection {
        UP, DOWN, LEFT, RIGHT
    };

    static std::unordered_map<MoveDirection, Position> MOVE_DIRECTION_MAP = {
        { UP,       {  -1, 0 }},
        { DOWN,     {  1, 0 }},
        { LEFT,     { 0, -1 }},
        { RIGHT,    {  0, 1 }}
    };

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

        State(const State & other) :
                board(Board(other.board)),
                isWhiteTurn(other.isWhiteTurn),
                cursor(Position(other.cursor)),
                currentMove(std::optional(other.currentMove)) {}

        void movePiece(const Position &begin, const Position &end);
        void swapPieces(const Position &begin, const Position &end);

        void moveCursor(const MoveDirection& dir);

        [[nodiscard]] const Piece & pieceAt(const Position & p) const {
            return board[p.first][p.second];
        }

        [[nodiscard]] Piece & pieceAt(const Position & p) {
            return board[p.first][p.second];
        }
    private:
        bool isPawnPromotion(const Position & end);
        void promotePawn(const Position & pos);
    };
}

#endif