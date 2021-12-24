#ifndef NCHESS_STATE_H
#define NCHESS_STATE_H

typedef vector<vector<Piece>> Board;

struct State
{
private:
    static Board initBoardState()
    {
        Board b =
        {
            {
                Piece(ROOK, true),
                        Piece(KNIGHT, true),
                        Piece(BISHOP, true),
                        Piece(KING, true),
                        Piece(QUEEN, true),
                        Piece(BISHOP, true),
                        Piece(KNIGHT, true),
                        Piece(ROOK, true)
            },
            {
                Piece(PAWN, true),
                        Piece(PAWN, true),
                        Piece(PAWN, true),
                        Piece(PAWN, true),
                        Piece(PAWN, true),
                        Piece(PAWN, true),
                        Piece(PAWN, true),
                        Piece(PAWN, true)
            }
        };
        for (int i = 0; i < 4; i++)
        {
            b.push_back({
                                 Piece(NONE),
                                 Piece(NONE),
                                 Piece(NONE),
                                 Piece(NONE),
                                 Piece(NONE),
                                 Piece(NONE),
                                 Piece(NONE),
                                 Piece(NONE)
            });
        }
        b.push_back
        ({
                         Piece(PAWN, false),
                         Piece(PAWN, false),
                         Piece(PAWN, false),
                         Piece(PAWN, false),
                         Piece(PAWN, false),
                         Piece(PAWN, false),
                         Piece(PAWN, false),
                         Piece(PAWN, false)
        });
        b.push_back
        ({
                         Piece(ROOK, false),
                         Piece(KNIGHT, false),
                         Piece(BISHOP, false),
                         Piece(KING, false),
                         Piece(QUEEN, false),
                         Piece(BISHOP, false),
                         Piece(KNIGHT, false),
                         Piece(ROOK, false)
        });

        return b;
    }
public:
    Board board;
    bool isWhiteTurn;
    Position cursor;
    optional<Position> currentMove;

    State() :
        board(initBoardState()),
        isWhiteTurn(true),
        cursor {0, 0},
        currentMove(nullopt)
    {}
};

void movePiece(Board& board, const Position& begin, const Position& end)
{
    board[end.first][end.second] = board[begin.first][begin.second];
    board[begin.first][begin.second] = NONE;
}

#endif