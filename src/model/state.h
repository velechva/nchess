#ifndef NCHESS_STATE_H
#define NCHESS_STATE_H

#include <vector>
#include <iostream>
#include <stdlib.h>

#include "piece.h"
#include "types.h"

namespace nchess::model {
    typedef std::vector<std::vector<Piece>> Board;

    static Position DELTA_UP    = { -1,  0 };
    static Position DELTA_DOWN  = {  1,  0 };
    static Position DELTA_LEFT  = {  0, -1 };
    static Position DELTA_RIGHT = {  0, -1 };

    struct State {
    public:
        Board board;
        bool isWhiteTurn;
        Position cursor;
        std::optional<Position> currentMove;
        std::vector<Position> moves;

        Board initialBoardState();

        State() :
                board(initialBoardState()),
                isWhiteTurn(true),
                cursor{0, 0},
                currentMove(std::nullopt) {}

        void changePlayer() { isWhiteTurn = !isWhiteTurn; }

        void movePiece(const Position &begin, const Position &end);
        void swapPieces(const Position &begin, const Position &end);

        void moveCursor(const Position & dir);

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