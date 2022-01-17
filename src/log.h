#ifndef NCHESS_LOG_H
#define NCHESS_LOG_H

#include <fstream>

#include "env.h"

namespace nchess::log {
    static std::ofstream debug(std::string(nchess::env::E_HOME) + "/nchess-debug.log", std::ios_base::out | std::ios_base::app);
    static std::ofstream error(std::string(nchess::env::E_HOME) + "/nchess-error.log", std::ios_base::out | std::ios_base::app);
}

#endif