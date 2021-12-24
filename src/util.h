#ifndef NCHESS_UTIL_H
#define NCHESS_UTIL_H

int sign(int delta)
{
    if (delta == 0) { return 0; }
    return delta > 0 ? 1 : -1;
}

Position makeDelta(const Position& begin, const Position& end)
{
    return
    {
        end.first - begin.first,
        end.second - begin.second
    };
}

#endif
