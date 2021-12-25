#ifndef NCHESS_STATE_H
#define NCHESS_STATE_H

typedef vector<vector<Piece>> Board;

#include <stdlib.h>
#include <iostream>

#include "file.h"

struct State
{
public:
    Board board;
    bool isWhiteTurn;
    Position cursor;
    optional<Position> currentMove;

    State() :
        board(initialBoardState()),
        isWhiteTurn(true),
        cursor {0, 0},
        currentMove(nullopt)
    {}

    Board initialBoardState();

    void movePiece(const Position& begin, const Position& end);

    [[nodiscard]] const Piece& pieceAt(const Position& p) const
    {
        return board[p.first][p.second];
    }
};

void State::movePiece(const Position& begin, const Position& end)
{
    board[end.first][end.second] = board[begin.first][begin.second];
    board[begin.first][begin.second] = NONE;
}

Board State::initialBoardState()
{
    Board board;

    vector<vector<string>> boardFile = read_csv("/Users/victorvelechosky/CLionProjects/nchess/res/board.txt");

    size_t i = 0;
    size_t j = 0;

    for (auto& line : boardFile)
    {
        vector<Piece> row;
        for (auto& el : line) {
            const char &c1 = el[0];
            if (c1 == '6')
            {
                row.push_back(Piece(NONE));
            }
            else
            {
                row.push_back(Piece(Type(KING + atoi(&el[1])), el[0] == 'W'));
            }
        }
        board.push_back(row);
    }

    return board;
}

#endif