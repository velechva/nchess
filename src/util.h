#ifndef NCHESS_UTIL_H
#define NCHESS_UTIL_H

int sign(int delta)
{
    if (delta == 0) { return 0; }
    return delta > 0 ? 1 : -1;
}

vector<string> split(const string& str, const char& delimiter)
{
    string padStr = str + delimiter;

    vector<string> output;
    size_t begin = 0;
    size_t end   = 0;

    while (end < padStr.size())
    {
        const char& c = padStr.at(end);

        if (c == delimiter && end > begin)
        {
            output.push_back(padStr.substr(begin, end - begin));
        }
        if (c == delimiter)
        {
            begin = ++end;
        }
        else
        {
            end++;
        }
    }

    return output;
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
