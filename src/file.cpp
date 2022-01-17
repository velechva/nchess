#include "file.h"

#include "log.h"

std::optional<std::vector<std::vector<std::string>>> nchess::file::read_csv(const std::string &fileName, const char &delimiter) {
    std::vector<std::vector<std::string>> output;

    nchess::log::error << "Enter " << fileName << "\n";

    std::ifstream fin(fileName);
    if (!fin.is_open()) { return std::nullopt; }

    std::string line;
    while (getline(fin, line)) {
        std::vector<std::string> tokens = nchess::util::split(line, ' ');
        output.push_back(tokens);
    }

    return std::optional(output);
}