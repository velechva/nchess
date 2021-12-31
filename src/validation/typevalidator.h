#ifndef NCHESS_TYPEVALIDATOR_H
#define NCHESS_TYPEVALIDATOR_H

#include "../util.h"

namespace nchess::validation {
    typedef std::function<bool(
            const nchess::model::State &,
            const nchess::model::Position &,
            const nchess::model::Position &
    )> Validator;

    bool validateNoBlockingPieces(const nchess::model::State &state, const nchess::model::Position &begin,
                                  const nchess::model::Position &end, const nchess::model::Position &delta) {
        const nchess::model::Position signs = nchess::util::sign(delta);
        nchess::model::Position p = nchess::util::sum(begin, signs);

        while (p != end) {
            if (state.pieceAt(p).kind != nchess::model::NONE) { return false; }

            p.first += signs.first;
            p.second += signs.second;
        }

        return true;
    }

    bool validateRook(const nchess::model::State &state, const nchess::model::Position &begin,
                      const nchess::model::Position &end) {
        const nchess::model::Position delta = nchess::util::difference(begin, end);

        // Rook only moves along x or y. Either deltaX XOR deltaY should be non-zero
        if ((delta.first == 0) == (delta.second == 0)) { return false; }

        return validateNoBlockingPieces(state, begin, end, delta);
    }

    bool validateBishop(const nchess::model::State &state, const nchess::model::Position &begin,
                        const nchess::model::Position &end) {
        const nchess::model::Board &board = state.board;

        const nchess::model::Position delta = nchess::util::difference(begin, end);
        const nchess::model::Position absDelta = {abs(delta.first), abs(delta.second)};

        // Bishop moves along diagonals
        if (delta.first == 0 || absDelta.first != absDelta.second) { return false; }

        return validateNoBlockingPieces(state, begin, end, delta);
    }

    bool validateKnight(const nchess::model::State &state, const nchess::model::Position &begin,
                        const nchess::model::Position &end) {
        nchess::model::Position delta = nchess::util::difference(begin, end);

        // Knights must move in L shapes
        return (
                (abs(delta.first) == 1 && abs(delta.second) == 2) ||
                (abs(delta.first) == 2 && abs(delta.second) == 1));
    }

    bool validateQueen(const nchess::model::State &state, const nchess::model::Position &begin,
                       const nchess::model::Position &end) {
        // Queen can move either like a bishop or a rook
        return validateRook(state, begin, end) || validateBishop(state, begin, end);
    }

    static std::vector<nchess::model::Position> KING_DELTAS = {
            {-1, 0},
            {0,  -1},
            {1,  0},
            {0,  1},
            {1,  1},
            {-1, -1},
            {1,  -1},
            {-1, 1}
    };

    bool validateKing(const nchess::model::State &state, const nchess::model::Position &begin,
                      const nchess::model::Position &end) {
        nchess::model::Position delta = nchess::util::difference(begin, end);

        for (auto &allowed: KING_DELTAS) {
            if (allowed == delta) { return true; }
        }

        return false;
    }

    bool validatePawn(const nchess::model::State &state, const nchess::model::Position &begin,
                      const nchess::model::Position &end) {
        const nchess::model::Board &board = state.board;

        const nchess::model::Position delta = nchess::util::difference(begin, end);

        int direction = state.isWhiteTurn ? 1 : -1;

        // Pawn moves diagonally to take an enemy piece
        if (
                delta.first == direction &&
                (delta.second == 1 || delta.second == -1) &&
                board[end.first][end.second].kind != nchess::model::NONE &&
                board[end.first][end.second].isWhite != state.isWhiteTurn)
        {
            return true;
        }

        // Pawn moves forward once
        else if (
                delta.first == direction &&
                delta.second == 0 &&
                board[end.first][end.second].kind == nchess::model::NONE)
        {
            return true;
        }

        // Pawn moves forward twice
        else if (
                ((state.isWhiteTurn && begin.first == 1) || (!state.isWhiteTurn && begin.first == 6)) &&
                delta.first == 2 * direction &&
                delta.second == 0 &&
                board[begin.first + direction][begin.second].kind == nchess::model::NONE &&
                board[end.first][end.second].kind == nchess::model::NONE)
        {
            return true;
        }

        // En passant TODO

        return false;
    }

    const std::unordered_map<nchess::model::Type, Validator> TYPE_VALIDATORS = {
        {nchess::model::ROOK,   validateRook},
        {nchess::model::BISHOP, validateBishop},
        {nchess::model::KNIGHT, validateKnight},
        {nchess::model::PAWN,   validatePawn},
        {nchess::model::QUEEN,  validateQueen},
        {nchess::model::KING,   validateKing}
    };
}

#endif