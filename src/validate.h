#ifndef NCHESS_VALIDATE_H
#define NCHESS_VALIDATE_H

#include "state.h"
#include "util.h"

bool validateMoveBegin(const State& state, const Position& begin);
bool validateMoveEnd(const State& state, const Position& begin, const Position& end);

bool inCheck(State state, bool whitePlayer)
{
    Board& board = state.board;

    for (int i = 0; i < board.size(); i++)
    {
        for (int j = 0; j < board.size(); j++)
        {
            Position begin = { i, j };
            auto piece = board[i][j];
            if (piece.kind != NONE && piece.isWhite != state.isWhiteTurn)
            {
                for (int i2 = 0; i2 < board.size(); i2++)
                {
                    for (int j2 = 0; j2 < board.size(); j2++)
                    {
                        if (
                                board[i2][j2].isWhite == state.isWhiteTurn &&
                                board[i2][j2].kind == KING &&
                                validateMoveBegin(state, begin) &&
                                validateMoveEnd(state, begin, { i2, j2 }))
                        {
                            return true;
                        }
                    }
                }
            }
        }
    }

    return false;
}

bool validateMoveBegin(const State& state, const Position& begin)
{
    const Piece& p = state.board[begin.first][begin.second];

    // Cannot move nothing
    if (p.kind == NONE) { return false; }
    // Cannot move other players pieces
    if (state.isWhiteTurn != p.isWhite) { return false; }

    return true;
}

bool validatePawnMoveEnd(const State& state, const Position& begin, const Position& end)
{
    const Board& board = state.board;

    const Position delta = makeDelta(begin, end);

    int direction = state.isWhiteTurn ? 1 : -1;

    // Pawn moves diagonally to take an enemy piece
    if (
            delta.first == direction &&
            (delta.second == 1 || delta.second == -1) &&
            board[end.first][end.second].kind != NONE &&
            board[end.first][end.second].isWhite != state.isWhiteTurn)
    {
        return true;
    }

    // Pawn moves forward once
    else if (
            delta.first == direction &&
            delta.second == 0 &&
            board[end.first][end.second].kind == NONE)
    {
        return true;
    }
    // Pawn moves forward twice
    else if (
            ((state.isWhiteTurn && begin.first == 1) || (!state.isWhiteTurn && begin.first == 6)) &&
            delta.first == 2 * direction &&
            delta.second == 0 &&
            board[begin.first + direction][begin.second].kind == NONE &&
            board[end.first][end.second].kind == NONE)
    {
        return true;
    }

    return false;
}

bool validateKnightMoveEnd(const State& state, const Position& begin, const Position& end)
{
    Position delta = makeDelta(begin, end);

    // Knights must move in L shapes
    return (
            (abs(delta.first) == 1 && abs(delta.second) == 2) ||
            (abs(delta.first) == 2 && abs(delta.second) == 1));
}

bool validateNoPiecesInTheWay(const State& state, const Position& begin, const Position& end, const Position& delta)
{
    const Board& board = state.board;

    Position signs = { sign(delta.first), sign(delta.second) };
    Position p = { begin.first + signs.first, begin.second + signs.second };

    while(p != end)
    {
        if (board[p.first][p.second].kind != NONE) { return false; }

        p.first += signs.first;
        p.second += signs.second;
    }

    return true;
}

bool validateRookMoveEnd(const State& state, const Position& begin, const Position& end)
{
    Position delta = makeDelta(begin, end);

    // Rook only moves along x or y. Either deltaX XOR deltaY should be non-zero
    if ((delta.first == 0) == (delta.second == 0)) { return false; }

    return validateNoPiecesInTheWay(state, begin, end, delta);
}

bool validateBishopMoveEnd(const State& state, const Position& begin, const Position& end)
{
    const Board& board = state.board;

    Position delta = makeDelta(begin, end);
    Position absDelta = { abs(delta.first), abs(delta.second) };

    // Bishop moves along diagonals
    if (delta.first == 0 || absDelta.first != absDelta.second) { return false; }

    return validateNoPiecesInTheWay(state, begin, end, delta);
}

bool validateMoveEnd(const State& state, const Position& begin, const Position& end)
{
    const Board& board = state.board;

    Piece pBegin = board[begin.first][begin.second];
    Piece pEnd = board[end.first][end.second];

    // Cannot replace your own piece
    if (pEnd.kind != NONE && state.isWhiteTurn == pEnd.isWhite) { return false; }

    if ((pBegin.kind == PAWN)   && !validatePawnMoveEnd(state, begin, end))    { return false; }
    if ((pBegin.kind == BISHOP) && !validateBishopMoveEnd(state, begin, end))  { return false; }
    if ((pBegin.kind == KNIGHT) && !validateKnightMoveEnd(state, begin, end))  { return false; }
    if ((pBegin.kind == ROOK)   && !validateRookMoveEnd(state, begin, end))    { return false; }

    Board newBoard(board);
    movePiece(newBoard, begin, end);

    // Cannot put yourself in check
//    if (inCheck(state, true))
//    {
//        return false;
//    }

    return true;
}

#endif
