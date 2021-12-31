#ifndef NCHESS_FILE_H
#define NCHESS_FILE_H

#include <iostream>
#include <optional>
#include <string>
#include <fstream>

#include "util.h"

namespace nchess::file {
    std::optional<std::vector<std::vector<std::string>>> read_csv(const std::string &fileName, const char &delimiter = ' ');
}

#endif