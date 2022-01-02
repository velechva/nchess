#include "util.h"

/**
 * Iterate with two pointers that represent the beginning and end
 * of the current token substring. Once a delimiter is hit,
 * push the current substring onto the stack (if it's non-empty)
 * otherwise keep moving forward. If the current character is not the delimiter,
 * increase the size of the current token substring
 */
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