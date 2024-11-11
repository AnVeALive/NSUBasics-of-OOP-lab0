#pragma once
#include <iostream>
#include <string>
#include <vector>
#include "CommandStructure.h"



class Config {
private:
    std::vector<Cmd> _cmds;

public:
    Config(const std::vector<Cmd> &&cmds);
    const Cmd &getCmd(const int index) const;
    int getSize() const;
};



class ConfigParser {
public:
    std::vector<Cmd> parse(const std::string &configFileName);
};