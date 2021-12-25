#ifndef NCHESS_FILE_H
#define NCHESS_FILE_H

#include <iostream>
#include <fstream>

#include "using.h"
#include "util.h"

vector<vector<string>> read_csv(const string& fileName, const char & delimiter = ' ')
{
    vector<vector<string>> output;

    std::ifstream fin(fileName);
    if (!fin.is_open()) { return {}; }

    string line;
    while(getline(fin, line))
    {
        vector<string> tokens = split(line, ' ');
        output.push_back(tokens);
    }

    return output;
}

#endif //NCHESS_FILE_H
