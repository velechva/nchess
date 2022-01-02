#ifndef NCHESS_VALIDATE_H
#define NCHESS_VALIDATE_H

#include <memory>

#include "typevalidator.h"
#include "../model/state.h"

namespace nchess::validation {
    /**
     * Check whether the current user is now in check
     */
    bool inCheck(nchess::model::State& state);

    /**
     * Check whether the current user is not in checkmate
     */
    bool inCheckMate(nchess::model::State& state);

    /**
     * Validate some basic rules in the beginning of a move
     * to allow early feedback to the user
     */
    bool validateMoveBegin(const nchess::model::State& state, const nchess::model::Position& begin);

    /**
     * Validate the end of a move
     * @param check whether to use in check detection
     * @return whether the move is valid at the current state
     */
    bool validateMoveEnd(const nchess::model::State& state, const nchess::model::Position& begin, const nchess::model::Position& end, const bool& check);

    typedef std::function<bool(
            const nchess::model::State &,
            const nchess::model::Position &,
            const nchess::model::Position &
    )> Validator;

    bool validateNoBlockingPieces(const nchess::model::State &state, const nchess::model::Position &begin,
                                  const nchess::model::Position &end, const nchess::model::Position &delta);

    bool validateRook(const nchess::model::State &state, const nchess::model::Position &begin,
                      const nchess::model::Position &end);

    bool validateBishop(const nchess::model::State &state, const nchess::model::Position &begin,
                        const nchess::model::Position &end);

    bool validateKnight(const nchess::model::State &state, const nchess::model::Position &begin,
                        const nchess::model::Position &end);

    bool validateQueen(const nchess::model::State &state, const nchess::model::Position &begin,
                       const nchess::model::Position &end);

    bool validateCastle(const nchess::model::State &state, const nchess::model::Position &begin,
                        const nchess::model::Position& end);

    bool validateKing(const nchess::model::State &state, const nchess::model::Position &begin,
                      const nchess::model::Position &end);

    bool validatePawn(const nchess::model::State &state, const nchess::model::Position &begin,
                      const nchess::model::Position &end);

    const std::unordered_map<nchess::model::Type, Validator> TYPE_VALIDATORS = {
            {nchess::model::ROOK,   validateRook},
            {nchess::model::BISHOP, validateBishop},
            {nchess::model::KNIGHT, validateKnight},
            {nchess::model::PAWN,   validatePawn},
            {nchess::model::QUEEN,  validateQueen},
            {nchess::model::KING,   validateKing}
    };
}

#endif