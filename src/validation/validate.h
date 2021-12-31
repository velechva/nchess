#ifndef NCHESS_VALIDATE_H
#define NCHESS_VALIDATE_H

#include <memory>

#include "../model/state.h"
#include "typevalidator.h"

namespace nchess::validation {
    bool validateMoveBegin(const nchess::model::State& state, const nchess::model::Position& begin);
    bool validateMoveEnd(const nchess::model::State& state, const nchess::model::Position& begin, const nchess::model::Position& end, const bool& check);

    bool inCheck(nchess::model::State& state)
    {
        state.isWhiteTurn = !state.isWhiteTurn;

        const nchess::model::Board& board = state.board;

        for (size_t i1 = 0; i1 < board.size(); i1++)
        {
            for (int j1 = 0; j1 < board.size(); j1++)
            {
                const nchess::model::Piece& piece = board[i1][j1];

                if (piece.kind == nchess::model::NONE || piece.isWhite != state.isWhiteTurn) { continue; }

                for (int i2 = 0; i2 < board.size(); i2++)
                {
                    for (int j2 = 0; j2 < board.size(); j2++)
                    {
                        // For each of the opposing players possible moves,
                        // if the move is valid and takes the current players king,
                        // this is a valid-checkable move
                        const nchess::model::Piece& destination = board[i2][j2];

                        const nchess::model::Position begin = { i1, j1 };
                        const nchess::model::Position end   = { i2, j2 };

                        if (
                                destination.isWhite != state.isWhiteTurn &&
                                destination.kind == nchess::model::KING &&
                                validateMoveBegin(state, begin) &&
                                validateMoveEnd(state, begin, end, false))
                        {
                            return true;
                        }
                    }
                }
            }
        }
        return false;
    }

    bool inCheckMate(nchess::model::State& state)
    {
        if (!inCheck(state)) { return false; }

        state.isWhiteTurn = !state.isWhiteTurn;

        const nchess::model::Board& board = state.board;

        for (int i1 = 0; i1 < board.size(); i1++)
        {
            for (int j1 = 0; j1 < board.size(); j1++)
            {
                const nchess::model::Piece& piece = board[i1][j1];

                if (piece.kind == nchess::model::NONE || piece.isWhite != state.isWhiteTurn || !validateMoveBegin(state, { i1, j1 })) { continue; }

                for (int i2 = 0; i2 < board.size(); i2++)
                {
                    for (int j2 = 0; j2 < board.size(); j2++)
                    {
                        const nchess::model::Piece& destination = board[i2][j2];

                        const nchess::model::Position begin = { i1, j1 };
                        const nchess::model::Position end   = { i2, j2 };

                        if (
                                destination.isWhite != state.isWhiteTurn &&
                                destination.kind != nchess::model::NONE &&
                                validateMoveBegin(state, begin) &&
                                validateMoveEnd(state, begin, end, false))
                        {
                            nchess::model::State newState(state);
                            newState.movePiece(begin, end);

                            if (!inCheck(newState))
                            {
                                return false;
                            }
                        }
                    }
                }
            }
        }
        return true;
    }

    bool validateMoveBegin(const nchess::model::State& state, const nchess::model::Position& begin)
    {
        const nchess::model::Piece& p = state.pieceAt(begin);

        // Cannot move nothing
        if (p.kind == nchess::model::NONE) { return false; }
        // Cannot move other players pieces
        if (state.isWhiteTurn != p.isWhite) { return false; }

        return true;
    }

    bool validateMoveEnd(const nchess::model::State& state, const nchess::model::Position& begin, const nchess::model::Position& end, const bool& check)
    {
        const nchess::model::Piece& pBegin  = state.pieceAt(begin);
        const nchess::model::Piece& pEnd    = state.pieceAt(end);

        // Cannot replace your own piece
        if (pEnd.kind != nchess::model::NONE && state.isWhiteTurn == pEnd.isWhite) { return false; }

        if (!TYPE_VALIDATORS.at(pBegin.kind)(state, begin, end)) { return false; }

        if (check)
        {
            nchess::model::State newState(state);
            newState.movePiece(begin, end);

            if (inCheck(newState)) {
                return false;
            }
        }

        return true;
    }
}

#endif