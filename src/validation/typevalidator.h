#ifndef NCHESS_TYPEVALIDATOR_H
#define NCHESS_TYPEVALIDATOR_H

class IMoveValidator
{
public:
    ~IMoveValidator() = default;

    virtual bool validate(const State& state, const Position& begin, const Position& end) = 0;
protected:
    static bool validateNoBlockingPieces(const State& state, const Position& begin, const Position& end, const Position& delta)
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
};

class RookValidator : public IMoveValidator
{
public:
    static bool validate(const State& state, const Position& begin, const Position& end) override
    {
        const Position delta = makeDelta(begin, end);

        // Rook only moves along x or y. Either deltaX XOR deltaY should be non-zero
        if ((delta.first == 0) == (delta.second == 0)) { return false; }

        return validateNoBlockingPieces(state, begin, end, delta);
    }
};

class BishopValidator : public IMoveValidator
{
public:
    static bool validate(const State& state, const Position& begin, const Position& end) override
    {
        const Board& board = state.board;

        const Position delta = makeDelta(begin, end);
        const Position absDelta = { abs(delta.first), abs(delta.second) };

        // Bishop moves along diagonals
        if (delta.first == 0 || absDelta.first != absDelta.second) { return false; }

        return validateNoBlockingPieces(state, begin, end, delta);
    }
};

class KnightValidator : public IMoveValidator
{
public:
    static bool validate(const State& state, const Position& begin, const Position& end) override
    {
        Position delta = makeDelta(begin, end);

        // Knights must move in L shapes
        return (
                (abs(delta.first) == 1 && abs(delta.second) == 2) ||
                (abs(delta.first) == 2 && abs(delta.second) == 1));
    }
};

class QueenValidator : public IMoveValidator
{
public:
    static bool validate(const State& state, const Position& begin, const Position& end) override
    {
        return RookValidator::validate(state, begin, end) || BishopValidator::validate(state, begin, end);
    }
};

class PawnValidator : public IMoveValidator
{
public:
    static bool validate(const State& state, const Position& begin, const Position& end) override
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
};

const unordered_map<Type, std::unique_ptr<IMoveValidator>> TYPE_I_MOVE_VALIDATOR_MAP =
{
                { ROOK,     std::make_unique<RookValidator>     (RookValidator())   },
                { BISHOP,   std::make_unique<BishopValidator>   (BishopValidator()) },
                { KNIGHT,   std::make_unique<KnightValidator>   (KnightValidator()) },
                { PAWN,     std::make_unique<PawnValidator>     (PawnValidator())   },
                { QUEEN,    std::make_unique<QueenValidator>     (QueenValidator())   },
};

#endif
