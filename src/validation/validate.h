#ifndef NCHESS_VALIDATE_H
#define NCHESS_VALIDATE_H

#include <memory>

#include "../model/state.h"
#include "typevalidator.h"
#include "../util.h"
#include "../log.h"

bool validateMoveBegin(const State& state, const Position& begin);
bool validateMoveEnd(const State& state, const Position& begin, const Position& end, const bool& check = true);
bool inCheck(const State& state);

// Check if (`whitePlayer` ? white : black) is in check with the board
// configured as `board`
bool inCheck(const State& state)
{
    const Board& board = state.board;

    for (size_t i = 0; i < board.size(); i++)
    {
        for (int j = 0; j < board.size(); j++)
        {
            const Piece& piece = board[i][j];

            logFile << "A: " << i << j << "\n";

            if (piece.kind == NONE || piece.isWhite != state.isWhiteTurn) { continue; }

            logFile << "B: " << i << j << "\n";

            const Position begin = { i, j };

            for (int i2 = 0; i2 < board.size(); i2++)
            {
                for (int j2 = 0; j2 < board.size(); j2++)
                {
                    logFile << "C: " << i2 << j2 << "\n";
                    // For each of the opposing players possible moves,
                    // if the move is valid and takes the current players king,
                    // this is a valid-checkable move
                    const Piece& destination = board[i2][j2];
                    if (
                            destination.isWhite != state.isWhiteTurn &&
                            destination.kind == KING &&
                            validateMoveBegin(state, begin) &&
                            validateMoveEnd(state, begin, { i2, j2 }, false))
                    {
                        logFile << "D: " << i2 << j2 << "\n";
                        return true;
                    }
                }
            }
        }
    }

    return false;
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

    if (!TYPE_I_MOVE_VALIDATOR_MAP.at(pBegin.kind)->validate(state, begin, end)) { return false; }

    State newState(state);
    newState.isWhiteTurn = !state.isWhiteTurn;
    newState.movePiece(begin, end);

    // Cannot put yourself in check
    if (check && inCheck(newState))
    {
        return false;
    }

    return true;
}

#endif