#ifndef NCHESS_PIECE_H
#define NCHESS_PIECE_H

#include <string>
#include <unordered_map>

using std::string;
using std::unordered_map;

enum Kind
{
    KING,
    QUEEN,
    BISHOP,
    ROOK,
    PAWN,
    KNIGHT,
    NONE
};

const unordered_map<Kind, const char *> KIND_DISPLAY_MAP =
        {
                {KING,   "K"},
                {QUEEN,  "Q"},
                {BISHOP, "B"},
                {ROOK,   "R"},
                {PAWN,   "P"},
                {KNIGHT, "N"},
                {NONE,   "-"}
        };

struct Piece
{
    Kind kind;
    bool isWhite;

    Piece(Kind _kind, bool _isWhite) : kind(_kind), isWhite(_isWhite) {}
    Piece(Kind _kind) : kind(_kind), isWhite(false) {}

    [[nodiscard]] const char * toString() const
    {
        return KIND_DISPLAY_MAP.at(kind);
    }
};

#endif