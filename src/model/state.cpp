#include "state.h"

#include "../file.h"

nchess::model::Board nchess::model::State::initialBoardState() {
    Board board;

    const auto boardFile = nchess::util::unwrapOrExit(
        nchess::file::read_csv("/Users/victorvelechosky/CLionProjects/nchess/res/board.txt"),
        "Failed to load board config");

    for (auto& line: boardFile) {
        std::vector<Piece> row;

        for (auto& el: line) {
            const char c1 = el[0];
            if (c1 == '6') {
                row.push_back(Piece(NONE));
            } else {
                row.push_back(Piece(Type(KING + atoi(&el[1])), el[0] == 'W'));
            }
        }
        board.push_back(row);
    }
    return board;
}

void nchess::model::State::movePiece(const Position &begin, const Position &end) {
    board[end.first][end.second] = board[begin.first][begin.second];
    board[end.first][end.second].movesMade++;
    board[begin.first][begin.second] = NONE;

    // Pawn promotion TODO allow user to select a piece?
    if (( isWhiteTurn && end.first == 7) ||
        (!isWhiteTurn && end.first == 0))
    {
        Piece& p = pieceAt(end);
        p.kind = QUEEN;
    }
}

void nchess::model::State::moveCursor(const MoveDirection& dir) {
    const Position & delta = MOVE_DIRECTION_MAP[dir];

    cursor.first  = std::clamp(cursor.first + delta.first, 0, 7);
    cursor.second = std::clamp(cursor.second + delta.second, 0, 7);
}