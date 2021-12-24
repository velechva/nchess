#ifndef NCHESS_UI_H
#define NCHESS_UI_H

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

void printControls()
{
    printw("w: up, a: left, s: down, d: right, m: move, e: exit\n");
}

void printError(const State& state, const char* str)
{
    move(12, 0);
    printw(str);
    refresh();
    move(state.cursor.first, state.cursor.second);
    getch();
}

void printBoard(const State& state)
{
    auto board = state.board;

    for (auto & i : board)
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
                printw(j.toString());
                attroff(COLOR_PAIR(1));
            }
            else
            {
                attron(COLOR_PAIR(2));
                printw(j.toString());
                attroff(COLOR_PAIR(2));
            }
        }
        printw("\n");
    }
    printw("\n\n");
}

void refreshUi(const State& state)
{
    clear();
    printBoard(state);
    printControls();
    refresh();
    move(state.cursor.first, state.cursor.second);
}

void exitApp()
{
    clear();
    refresh();
    endwin();
    exit(0);
}

#endif