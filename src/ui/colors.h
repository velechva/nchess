#ifndef NCHESS_COLORS_H
#define NCHESS_COLORS_H

#include <ncurses.h>

namespace nchess::ui {
    enum COLOR_CODE {
        BLACK       = COLOR_BLACK,
        RED	        = COLOR_RED,
        GREEN	    = COLOR_GREEN,
        YELLOW	    = COLOR_YELLOW,
        BLUE	    = COLOR_BLUE,
        MAGENTA	    = COLOR_MAGENTA,
        CYAN	    = COLOR_CYAN,
        WHITE	    = COLOR_WHITE,
    };

    void setupColors() {
        if (has_colors() == FALSE) {
            printf("Terminal does not support colors.");
            ::exit(1);
        }

        start_color();

        init_pair(COLOR_BLACK, COLOR_BLACK, COLOR_BLACK);
        init_pair(COLOR_RED, COLOR_RED, COLOR_BLACK);
        init_pair(COLOR_GREEN, COLOR_GREEN, COLOR_BLACK);
        init_pair(COLOR_YELLOW, COLOR_YELLOW, COLOR_BLACK);
        init_pair(COLOR_BLUE, COLOR_BLUE, COLOR_BLACK);
        init_pair(COLOR_MAGENTA, COLOR_MAGENTA, COLOR_BLACK);
        init_pair(COLOR_CYAN, COLOR_CYAN, COLOR_BLACK);
        init_pair(COLOR_WHITE, COLOR_WHITE, COLOR_BLACK);
    }

    void colorOn(COLOR_CODE code) {
       attron(COLOR_PAIR(code));
    }

    void colorOff(COLOR_CODE code) {
        attroff(COLOR_PAIR(code));
    }

    void print(const char * msg) {
        printw(msg);
    }

    void print(const char * msg, COLOR_CODE color) {
        colorOn(color);
        printw(msg);
        colorOff(color);
    }
}

#endif