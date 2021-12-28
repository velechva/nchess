#ifndef NCHESS_TYPEVALIDATOR_H
#define NCHESS_TYPEVALIDATOR_H

typedef std::function<bool(const State&, const Position&, const Position&)> Validator;

bool validateNoBlockingPieces(const State& state, const Position& begin, const Position& end, const Position& delta)
{
    const Position signs = { sign(delta.first), sign(delta.second) };
    Position p = { begin.first + signs.first, begin.second + signs.second };

    while(p != end)
    {
        if (state.pieceAt(p).kind != NONE) { return false; }

        p.first += signs.first;
        p.second += signs.second;
    }

    return true;
}

bool validateRook(const State& state, const Position& begin, const Position& end)
{
    const Position delta = makeDelta(begin, end);

    // Rook only moves along x or y. Either deltaX XOR deltaY should be non-zero
    if ((delta.first == 0) == (delta.second == 0)) { return false; }

    return validateNoBlockingPieces(state, begin, end, delta);
}

bool validateBishop(const State& state, const Position& begin, const Position& end)
{
    const Board& board = state.board;

    const Position delta = makeDelta(begin, end);
    const Position absDelta = { abs(delta.first), abs(delta.second) };

    // Bishop moves along diagonals
    if (delta.first == 0 || absDelta.first != absDelta.second) { return false; }

    return validateNoBlockingPieces(state, begin, end, delta);
}

bool validateKnight(const State& state, const Position& begin, const Position& end)
{
    Position delta = makeDelta(begin, end);

    // Knights must move in L shapes
    return (
            (abs(delta.first) == 1 && abs(delta.second) == 2) ||
            (abs(delta.first) == 2 && abs(delta.second) == 1));
}

bool validateQueen(const State& state, const Position& begin, const Position& end)
{
    // Queen can move either like a bishop or a rook
    return validateRook(state, begin, end) || validateBishop(state, begin, end);
}

bool validateKing(const State& state, const Position& begin, const Position& end)
{
    Position delta = makeDelta(begin, end);
    vector<Position> kingDeltas =
    {
            {-1,0},{0,-1},{1,0},{0,1},{1,1},{-1,-1},{1,-1},{-1,1}
    };

    for (auto& allowed : kingDeltas)
    {
        if (allowed == delta) { return true; }
    }

    return false;
}

bool validatePawn(const State& state, const Position& begin, const Position& end)
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

const unordered_map<Type, Validator> TYPE_VALIDATORS =
{
                { ROOK,     validateRook    },
                { BISHOP,   validateBishop  },
                { KNIGHT,   validateKnight  },
                { PAWN,     validatePawn    },
                { QUEEN,    validateQueen   },
                { KING,     validateKing    }
};

#endif