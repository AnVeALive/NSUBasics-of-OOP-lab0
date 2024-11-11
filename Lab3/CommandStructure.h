#pragma once
#include <string>
#include <vector>



typedef struct Command {
    std::string name;
    std::vector<int> args;
} Cmd;