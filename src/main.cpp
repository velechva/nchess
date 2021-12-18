#include <ncurses.h>
#include <cstdlib>
#include <utility>
#include <algorithm>
#include <optional>
#include <vector>

using std::pair, std::max, std::min, std::optional, std::nullopt, std::vector;

typedef pair<int, int> Position;

Position makeDelta(Position begin, Position end)
{
    return { end.first - begin.first, end.second - begin.second };
}

int sign(int delta)
{
    if (delta == 0) { return 0; }
    return delta > 0 ? 1 : -1;
}

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

struct Piece
{
    Kind kind;
    bool isWhite;

    Piece(Kind _kind, bool _isWhite) : kind(_kind), isWhite(_isWhite) { }
    Piece(Kind _kind) : kind(_kind), isWhite(false) { }

    void print()
    {
        if (kind == KING)
        {
            printw("K");
        }
        else if (kind == QUEEN)
        {
            printw("Q");
        }
        else if (kind == ROOK)
        {
            printw("R");
        }
        else if (kind == BISHOP)
        {
            printw("B");
        }
        else if (kind == KNIGHT)
        {
            printw("N");
        }
        else if (kind == PAWN)
        {
            printw("P");
        }
    }
};

typedef vector<vector<Piece>> State;
State initBoardState()
{
    State st =
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
        st.push_back({
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
    st.push_back
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
    st.push_back
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
    return st;
}

bool whiteTurn = true;

State state = initBoardState();

Position cursor = { 0, 0 };
optional<Position> currentMove = nullopt;

bool inCheck(bool whitePlayer, State st)
{
    for (int i = 0; i < state.size(); i++)
    {
        for (int j = 0; j < state.size(); j++)
        {
            Position begin = { i, j };
            auto piece = state[i][j];
            if (piece.kind != NONE && piece.isWhite != whiteTurn)
            {
                for (int i2 = 0; i2 < state.size(); i2++)
                {
                    for (int j2 = 0; j2 < state.size(); j2++)
                    {
                        if (
                            state[i2][j2].isWhite == whiteTurn &&
                            state[i2][j2].kind == KING &&
                            validateMoveBegin(begin) &&
                            validateMoveEnd(begin, { i2, j2 }))
                        {
                            return true;
                        }
                    }
                }
            }
        }
    }

    return false;
}

void printBoard() 
{
    for (auto & i : state) 
    {
        for (auto & j : i) 
        {
            if (j.kind == NONE) 
            {
                printw("-");
            }
            else if (j.isWhite) 
            {
                attron(COLOR_PAIR(1));
                j.print();
                attroff(COLOR_PAIR(1));
            }
            else 
            {
                attron(COLOR_PAIR(2));
                j.print();
                attroff(COLOR_PAIR(2));
            }
        }
        printw("\n");
    }
    printw("\n\n");
}

void validateEnvironment() 
{
    if (has_colors() == FALSE) 
    {
        printf("Terminal does not support colors.");
        exit(1);
    }
}

void setupColors() 
{
    start_color();
    init_pair(1, COLOR_WHITE, COLOR_BLACK);
    init_pair(2, COLOR_BLUE, COLOR_BLACK);
}

void exitApp() 
{
    clear();
    refresh();
    endwin();
    exit(0);
}

void printControls() 
{
    printw("w: up, a: left, s: down, d: right, m: move, e: exit\n");
}

bool validateMoveBegin(Position begin) 
{
    Piece p = state[begin.first][begin.second];

    // Cannot move nothing
    if (p.kind == NONE) { return false; }
    // Cannot move other players pieces
    if (whiteTurn != p.isWhite) { return false; }

    return true;
}

bool validatePawnMoveEnd(Position begin, Position end) 
{
    Position delta = makeDelta(begin, end);

    int direction = whiteTurn ? 1 : -1;

    // Pawn moves diagonally to take an enemy piece
    if (
            delta.first == direction &&
            (delta.second == 1 || delta.second == -1) &&
            state[end.first][end.second].kind != NONE &&
            state[end.first][end.second].isWhite != whiteTurn)
    {
        return true;
    }

    // Pawn moves forward once
    else if (delta.first == direction && delta.second == 0 && state[end.first][end.second].kind == NONE)
    {
        return true;
    }
    // Pawn moves forward twice
    else if (
            ((whiteTurn && begin.first == 1) || (!whiteTurn && begin.first == 6)) &&
            delta.first == 2 * direction &&
            delta.second == 0 &&
            state[begin.first + direction][begin.second].kind == NONE &&
            state[end.first][end.second].kind == NONE)
    {
        return true;
    }

    return false;
}

bool validateKnightMoveEnd(Position begin, Position end)
{
    Position delta = makeDelta(begin, end);

    // Knights must move in L shapes
    return (
            (abs(delta.first) == 1 && abs(delta.second) == 2) ||
            (abs(delta.first) == 2 && abs(delta.second) == 1));
}

bool validateNoPiecesInTheWay(Position begin, Position end, Position delta)
{
    Position signs = { sign(delta.first), sign(delta.second) };
    Position p = { begin.first + signs.first, begin.second + signs.second };

    while(p != end)
    {
        if (state[p.first][p.second].kind != NONE) { return false; }

        p.first += signs.first;
        p.second += signs.second;
    }

    return true;
}

bool validateRookMoveEnd(Position begin, Position end)
{
    Position delta = makeDelta(begin, end);

    // Rook only moves along x or y. Either deltaX XOR deltaY should be non-zero
    if ((delta.first == 0) == (delta.second == 0)) { return false; }

    return validateNoPiecesInTheWay(begin, end, delta);
}

bool validateBishopMoveEnd(Position begin, Position end)
{
    Position delta = makeDelta(begin, end);
    Position absDelta = { abs(delta.first), abs(delta.second) };

    // Bishop moves along diagonals
    if (delta.first == 0 || absDelta.first != absDelta.second) { return false; }

    return validateNoPiecesInTheWay(begin, end, delta);
}

void movePiece(State st, Position begin, Position end)
{
    st[end.first][end.second] = st[begin.first][begin.second];
    st[begin.first][begin.second] = NONE;
}

bool validateMoveEnd(Position begin, Position end)
{
    Piece pBegin = state[begin.first][begin.second];
    Piece pEnd = state[end.first][end.second];

    // Cannot replace your own piece
    if (pEnd.kind != NONE && whiteTurn == pEnd.isWhite) { return false; }

    if ((pBegin.kind == PAWN)   && !validatePawnMoveEnd(begin, end))    { return false; }
    if ((pBegin.kind == BISHOP) && !validateBishopMoveEnd(begin, end))  { return false; }
    if ((pBegin.kind == KNIGHT) && !validateKnightMoveEnd(begin, end))  { return false; }
    if ((pBegin.kind == ROOK)   && !validateRookMoveEnd(begin, end))    { return false; }

    State st(state);
    movePiece(st, begin, end);
    bool willBeInCheck = inCheck(whiteTurn, st);

    // Cannot put yourself in check
    if (willBeInCheck)
    {
        return false;
    }

    return true;
}

void printError(const char* str)
{
    move(12, 0);
    printw(str);
    refresh();
    move(cursor.first, cursor.second);
    getch();
}

void refreshUi()
{
    clear();
    printBoard();
    printControls();
    refresh();
    move(cursor.first, cursor.second);
}

int main()
{
    initscr();
    raw();
    noecho();
    validateEnvironment();
    setupColors();
    refreshUi();

    char ch = ' ';

    while(ch != 'e')
    {
        ch = getch();

        if (ch == 'e')
        {
            exitApp();
        }
        else if (ch == 'w')
        {
            cursor.first = max(0, cursor.first - 1);
        }
        else if (ch == 'a')
        {
            cursor.second = max(0, cursor.second - 1);
        }
        else if (ch == 's')
        {
            cursor.first = min(7, cursor.first + 1);
        }
        else if (ch == 'd')
        {
            cursor.second = min(7, cursor.second + 1);
        }
        else if (ch == 'm')
        {
            if (!currentMove)
            {
                if (!validateMoveBegin(cursor))
                {
                    printError("Invalid move");
                }
                else
                {
                    currentMove = cursor;
                }
            }
            else
            {
                if (!validateMoveEnd(currentMove.value(), cursor))
                {
                    printError("Invalid move");
                }
                else
                {
                    movePiece(state, currentMove.value(), cursor);
                    whiteTurn = !whiteTurn;
                }
                currentMove = nullopt;
            }
        }

        refreshUi();
    }
    exitApp();
}
