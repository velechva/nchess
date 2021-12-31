#include "util.h"

std::vector<std::string> nchess::util::split(const std::string &str, const char &delimiter) {
    std::vector<std::string> output;

    // TODO this is too expensive
    std::string padStr = str + delimiter;

    size_t begin = 0, end = 0;

    while (end < padStr.size()) {
        const char &c = padStr[end];

        if (c == delimiter && end > begin) {
            output.push_back(padStr.substr(begin, end - begin));
        }

        end++;

        if (c == delimiter) {
            begin = end;
        }
    }
    return output;
}
