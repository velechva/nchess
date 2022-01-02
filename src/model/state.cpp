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

/**
 * Promotes a pawn
 * TODO allow the user to select a type to promote?
 */
void nchess::model::State::promotePawn(const Position & pos) {
    pieceAt(pos).kind = QUEEN;
}

/**
 * Check if a pawn should be promoted
 */
bool nchess::model::State::isPawnPromotion(const Position & end) {
    Piece & p = pieceAt(end);
    // Only pawns can be promoted
    if (p.kind != PAWN) { return false; }
    // Pawns are only promoted when they've advanced to the enemy home rank
    return (( isWhiteTurn && end.first == 7) ||
            (!isWhiteTurn && end.first == 0));
}

void nchess::model::State::movePiece(const Position &begin, const Position &end) {
    board[end.first][end.second] = board[begin.first][begin.second];
    board[end.first][end.second].movesMade++;
    board[begin.first][begin.second] = NONE;

    if (isPawnPromotion(end)) {
        promotePawn(end);
    }
}

void nchess::model::State::moveCursor(const MoveDirection& dir) {
    const Position & delta = MOVE_DIRECTION_MAP[dir];

    cursor.first  = std::clamp(cursor.first + delta.first, 0, 7);
    cursor.second = std::clamp(cursor.second + delta.second, 0, 7);
}