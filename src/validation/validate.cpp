#include "validate.h"

static std::vector<nchess::model::Position> KING_DELTAS = {
        {-1,   0 },
        { 0,  -1 },
        { 1,   0 },
        { 0,   1 },
        { 1,   1 },
        {-1,  -1 },
        { 1,  -1 },
        {-1,   1 }
};

bool nchess::validation::inCheck(nchess::model::State &state) {
    state.isWhiteTurn = !state.isWhiteTurn;

    const nchess::model::Board &board = state.board;

    for (size_t i1 = 0; i1 < board.size(); i1++) {
        for (int j1 = 0; j1 < board.size(); j1++) {
            const nchess::model::Piece &piece = board[i1][j1];

            if (piece.kind == nchess::model::NONE || piece.isWhite != state.isWhiteTurn) { continue; }

            for (int i2 = 0; i2 < board.size(); i2++) {
                for (int j2 = 0; j2 < board.size(); j2++) {
                    // For each of the opposing players possible moves,
                    // if the move is valid and takes the current players king,
                    // this is a valid-checkable move
                    const nchess::model::Piece &destination = board[i2][j2];

                    const nchess::model::Position begin = {i1, j1};
                    const nchess::model::Position end = {i2, j2};

                    if (
                            destination.isWhite != state.isWhiteTurn &&
                            destination.kind == nchess::model::KING &&
                            validateMoveBegin(state, begin) &&
                            validateMoveEnd(state, begin, end, false)) {
                        return true;
                    }
                }
            }
        }
    }
    return false;
}

bool nchess::validation::inCheckMate(nchess::model::State &state) {
    if (!nchess::validation::inCheck(state)) { return false; }

    state.isWhiteTurn = !state.isWhiteTurn;

    const nchess::model::Board &board = state.board;

    for (int i1 = 0; i1 < board.size(); i1++) {
        for (int j1 = 0; j1 < board.size(); j1++) {
            const nchess::model::Piece &piece = board[i1][j1];

            if (piece.kind == nchess::model::NONE || piece.isWhite != state.isWhiteTurn ||
                !validateMoveBegin(state, {i1, j1})) { continue; }

            for (int i2 = 0; i2 < board.size(); i2++) {
                for (int j2 = 0; j2 < board.size(); j2++) {
                    const nchess::model::Piece &destination = board[i2][j2];

                    const nchess::model::Position begin = {i1, j1};
                    const nchess::model::Position end = {i2, j2};

                    if (
                            destination.isWhite != state.isWhiteTurn &&
                            destination.kind != nchess::model::NONE &&
                            validateMoveBegin(state, begin) &&
                            validateMoveEnd(state, begin, end, false)) {
                        nchess::model::State newState(state);
                        newState.movePiece(begin, end);

                        if (!inCheck(newState)) {
                            return false;
                        }
                    }
                }
            }
        }
    }
    return true;
}

bool nchess::validation::validateMoveBegin(const nchess::model::State &state, const nchess::model::Position &begin) {
    const nchess::model::Piece &p = state.pieceAt(begin);

    // Cannot move nothing
    if (p.kind == nchess::model::NONE) { return false; }
    // Cannot move other players pieces
    if (state.isWhiteTurn != p.isWhite) { return false; }

    return true;
}

bool nchess::validation::validateMoveEnd(const nchess::model::State &state, const nchess::model::Position &begin,
                     const nchess::model::Position &end, const bool &check) {
    const nchess::model::Piece &pBegin = state.pieceAt(begin);
    const nchess::model::Piece &pEnd = state.pieceAt(end);

    // Cannot replace your own piece
    if (pEnd.kind != nchess::model::NONE && state.isWhiteTurn == pEnd.isWhite) { return false; }
    // Validate against the specific Type validator
    if (!nchess::validation::TYPE_VALIDATORS.at(pBegin.kind)(state, begin, end)) { return false; }

    // TODO refactor - function does too much
    if (check) {
        nchess::model::State newState(state);
        newState.movePiece(begin, end);

        // Cannot put yourself in check
        if (inCheck(newState)) {
            return false;
        }
    }

    return true;
}

bool nchess::validation::validateNoBlockingPieces(const nchess::model::State &state, const nchess::model::Position &begin,
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

bool nchess::validation::validateRook(const nchess::model::State &state, const nchess::model::Position &begin,
                  const nchess::model::Position &end) {
    const nchess::model::Position delta = nchess::util::difference(begin, end);

    // Rook only moves along x or y. Either deltaX XOR deltaY should be non-zero
    if ((delta.first == 0) == (delta.second == 0)) { return false; }

    return nchess::validation::validateNoBlockingPieces(state, begin, end, delta);
}

bool nchess::validation::validateBishop(const nchess::model::State &state, const nchess::model::Position &begin,
                    const nchess::model::Position &end) {
    const nchess::model::Board &board = state.board;

    const nchess::model::Position delta = nchess::util::difference(begin, end);
    const nchess::model::Position absDelta = {abs(delta.first), abs(delta.second)};

    // Bishop moves along diagonals
    if (delta.first == 0 || absDelta.first != absDelta.second) { return false; }

    return nchess::validation::validateNoBlockingPieces(state, begin, end, delta);
}

bool nchess::validation::validateKnight(const nchess::model::State &state, const nchess::model::Position &begin,
                    const nchess::model::Position &end) {
    nchess::model::Position delta = nchess::util::difference(begin, end);

    // Knights must move in L shapes
    return (
            (abs(delta.first) == 1 && abs(delta.second) == 2) ||
            (abs(delta.first) == 2 && abs(delta.second) == 1));
}

bool nchess::validation::validateQueen(const nchess::model::State &state, const nchess::model::Position &begin,
                   const nchess::model::Position &end) {
    // Queen can move either like a bishop or a rook
    return nchess::validation::validateRook(state, begin, end) || nchess::validation::validateBishop(state, begin, end);
}

bool nchess::validation::validateCastle(const nchess::model::State &state, const nchess::model::Position &begin,
                    const nchess::model::Position& end) {
    const nchess::model::Piece& pBegin = state.pieceAt(begin);
    const nchess::model::Piece& pEnd   = state.pieceAt(end);

    // In a Castle, both pieces should be your own
    if (pBegin.isWhite != state.isWhiteTurn || pEnd.isWhite != state.isWhiteTurn) { return false; }
    // Starts with a King, ends with a Rook
    if (pBegin.kind != model::KING || pEnd.kind != model::ROOK) { return false; }
    // Pieces should be in its original position
    if (pBegin.movesMade != 0 && pEnd.movesMade != 0) { return false; }
    // Cannot use a Castle to get out of being in check
    nchess::model::State newState(state);
    if (nchess::validation::inCheck(newState)) { return false; }
    // No pieces can be in the way
    const nchess::model::Position delta = nchess::util::difference(begin, end);
    validateNoBlockingPieces(state, begin, end, delta);
    // No move along the way can put the King in check
    const nchess::model::Position sign = nchess::util::sign(delta);
//    while() {
//        newState.movePiece(begin, ...);
//        if (inCheck(newState)) { return false; }
//    }

    return true;
}

bool nchess::validation::validateKing(const nchess::model::State &state, const nchess::model::Position &begin,
                  const nchess::model::Position &end) {
    nchess::model::Position delta = nchess::util::difference(begin, end);

    for (auto &allowed: KING_DELTAS) {
        if (allowed == delta) { return true; }
    }

    if (nchess::validation::validateCastle(state, begin, end)) { return true; }

    return false;
}

bool nchess::validation::validatePawn(const nchess::model::State &state, const nchess::model::Position &begin,
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