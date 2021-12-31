#ifndef NCHESS_PIECE_H
#define NCHESS_PIECE_H

#include <string>
#include <unordered_map>

namespace nchess::model {
    enum Type {
        KING = 0,
        QUEEN = 1,
        BISHOP = 2,
        ROOK = 3,
        PAWN = 4,
        KNIGHT = 5,
        NONE = 6
    };

    static std::unordered_map<Type, const char *> TYPE_DISPLAY_MAP = {
            {KING,   "K"},
            {QUEEN,  "Q"},
            {BISHOP, "B"},
            {ROOK,   "R"},
            {PAWN,   "P"},
            {KNIGHT, "N"},
            {NONE,   "-"}
    };

    struct Piece {
        Type kind;
        bool isWhite;

        Piece(Type _kind, bool _isWhite) : kind(_kind), isWhite(_isWhite) {}

        Piece(Type _kind) : kind(_kind), isWhite(false) {}

        [[nodiscard]] const char *toString() const {
            return TYPE_DISPLAY_MAP.at(kind);
        }
    };
}

#endif