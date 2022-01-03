#ifndef NCHESS_LOG_H
#define NCHESS_LOG_H

#include <fstream>

namespace nchess::log {
    static std::ofstream debug("/Users/victorvelechosky/CLionProjects/nchess/debug.log", std::ios_base::out | std::ios_base::app);
    static std::ofstream error("/Users/victorvelechosky/CLionProjects/nchess/error.log", std::ios_base::out | std::ios_base::app);
}

#endif