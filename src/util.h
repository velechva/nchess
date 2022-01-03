#ifndef NCHESS_UTIL_H
#define NCHESS_UTIL_H

#include <vector>
#include <string>
#include <optional>

#include "log.h"

namespace nchess::util {
    /**
     * Sign function
     *
     * @return val < 0 ? -1 : 1
     */
    template<typename T>
    inline int sign(T val) {
        if (val < 0) { return -1; }
        if (val > 0) { return  1; }

        return 0;
    }

    /**
     * Apply Sign function to both parameters in a pair
     */
    template<typename T>
    inline std::pair<T, T> sign(const std::pair<T, T>& pair) {
        return {
                sign(pair.first),
                sign(pair.second)
        };
    }

    /**
     * Split a string into tokens
     *
     * @return an array of tokens from `str` delimited by `delimeter`
     */
    std::vector<std::string> split(const std::string &str, const char &delimiter);

    /**
     * Calculate difference between two pairs
     * @return end - begin
     */
    template<typename T>
    inline std::pair<T, T> difference(const std::pair<T, T> &begin, const std::pair<T, T> &end) {
        return {
                end.first  - begin.first,
                end.second - begin.second
        };
    }

    /**
     * Calculate addition between two pairs
     * @return begin + end
     */
     template<typename T>
     inline std::pair<T, T> sum(const std::pair<T, T>& begin, const std::pair<T, T> &end) {
        return {
                end.first  + begin.first,
                end.second + begin.second
        };
     }

     template<typename T>
     inline T unwrapOrExit(std::optional<T> opt, const char * exitMessage) {
         if (opt) {
             return opt.value();
         }

         nchess::log::error << exitMessage << "\n";
         exit(1);
     }
}

#endif