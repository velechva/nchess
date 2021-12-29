#ifndef NCHESS_VALIDATE_H
#define NCHESS_VALIDATE_H

#include <memory>

#include "../model/state.h"
#include "typevalidator.h"
#include "../util.h"
#include "../log.h"

bool validateMoveBegin(const State& state, const Position& begin);
bool validateMoveEnd(const State& state, const Position& begin, const Position& end, const bool& check);

bool inCheck(State& state)
{
    state.isWhiteTurn = !state.isWhiteTurn;

    const Board& board = state.board;

    for (size_t i1 = 0; i1 < board.size(); i1++)
    {
        for (int j1 = 0; j1 < board.size(); j1++)
        {
            const Piece& piece = board[i1][j1];

            if (piece.kind == NONE || piece.isWhite != state.isWhiteTurn) { continue; }

            for (int i2 = 0; i2 < board.size(); i2++)
            {
                for (int j2 = 0; j2 < board.size(); j2++)
                {
                    // For each of the opposing players possible moves,
                    // if the move is valid and takes the current players king,
                    // this is a valid-checkable move
                    const Piece& destination = board[i2][j2];

                    const Position begin = { i1, j1 };
                    const Position end   = { i2, j2 };

                    if (
                            destination.isWhite != state.isWhiteTurn &&
                            destination.kind == KING &&
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

bool inCheckMate(State& state)
{
    logFile << "hello" << "\n";
    if (!inCheck(state)) { return false; }

    logFile << "hello2" << "\n";
    state.isWhiteTurn = !state.isWhiteTurn;

    const Board& board = state.board;

    for (int i1 = 0; i1 < board.size(); i1++)
    {
        for (int j1 = 0; j1 < board.size(); j1++)
        {
            const Piece& piece = board[i1][j1];

            if (piece.kind == NONE || piece.isWhite != state.isWhiteTurn || !validateMoveBegin(state, { i1, j1 })) { continue; }

            for (int i2 = 0; i2 < board.size(); i2++)
            {
                for (int j2 = 0; j2 < board.size(); j2++)
                {
                    const Piece& destination = board[i2][j2];

                    const Position begin = { i1, j1 };
                    const Position end   = { i2, j2 };

                    if (
                            destination.isWhite != state.isWhiteTurn &&
                            destination.kind != NONE &&
                            validateMoveBegin(state, begin) &&
                            validateMoveEnd(state, begin, end, false))
                    {
                        State newState(state);
                        newState.movePiece(begin, end);

                        if (!inCheck(newState))
                        {
                            logFile << "Solution found " << "(" << begin.first << "," << begin.second << ") (" << end.first << "," << end.second << ")";
                            return false;
                        }
                    }
                }
            }
        }
    }
    return true;
}

bool validateMoveBegin(const State& state, const Position& begin)
{
    const Piece& p = state.pieceAt(begin);

    // Cannot move nothing
    if (p.kind == NONE) { return false; }
    // Cannot move other players pieces
    if (state.isWhiteTurn != p.isWhite) { return false; }

    return true;
}

bool validateMoveEnd(const State& state, const Position& begin, const Position& end, const bool& check)
{
    const Piece& pBegin  = state.pieceAt(begin);
    const Piece& pEnd    = state.pieceAt(end);

    // Cannot replace your own piece
    if (pEnd.kind != NONE && state.isWhiteTurn == pEnd.isWhite) { return false; }

    if (!TYPE_VALIDATORS.at(pBegin.kind)(state, begin, end)) { return false; }

    if (check)
    {
        State newState(state);
        newState.movePiece(begin, end);

        if (inCheck(newState)) {
            return false;
        }
    }

    return true;
}

#endif