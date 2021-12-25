#include <algorithm>
#include <optional>

#include <ncurses.h>

#include "types.h"

#include "piece.h"
#include "state.h"
#include "ui.h"
#include "validate.h"

int main()
{
    initscr();
    raw();
    noecho();
    validateEnvironment();
    setupColors();

    State state;

    refreshUi(state);

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
            state.cursor.first = max(0, state.cursor.first - 1);
        }
        else if (ch == 'a')
        {
            state.cursor.second = max(0, state.cursor.second - 1);
        }
        else if (ch == 's')
        {
            state.cursor.first = min(7, state.cursor.first + 1);
        }
        else if (ch == 'd')
        {
            state.cursor.second = min(7, state.cursor.second + 1);
        }
        else if (ch == 'm')
        {
            if (!state.currentMove)
            {
                if (!validateMoveBegin(state, state.cursor))
                {
                    printError(state, "Invalid move");
                }
                else
                {
                    state.currentMove = state.cursor;
                }
            }
            else
            {
                if (!validateMoveEnd(state, state.currentMove.value(), state.cursor))
                {
                    printError(state, "Invalid move");
                }
                else
                {
                    state.movePiece(state.currentMove.value(), state.cursor);
                    state.isWhiteTurn = !state.isWhiteTurn;
                }
                state.currentMove = nullopt;
            }
        }

        refreshUi(state);
    }
    exitApp();
}