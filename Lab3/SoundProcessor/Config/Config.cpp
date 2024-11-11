#include <fstream>
#include <sstream>
#include "Config.h"

using namespace std;



// && - ссылка на временные объект (rvalue).
// Конструктор принимает временный объект (rvalue).
// move - перемещает cmds в _cmds без копирования.
Config::Config(const vector<Cmd> &&cmds) : _cmds(std::move(cmds)) {}

// Получить комманду по индексу.
const Cmd &Config::getCmd(const int index) const {
    return _cmds.at(index);
}

// Получить кол-во команд.
int Config::getSize() const {
    return _cmds.size();
}



// Распарсить конфигурационный файл.
// Возвращает вектор команд.
vector<Cmd> ConfigParser::parse(const string &configFileName) {
    ifstream fin(configFileName);

    if (!fin.is_open()) {
        throw runtime_error("Failed to open file: " + configFileName);
    }

    vector<Cmd> cmds;

    string line;
    while (getline(fin, line)) {

        if (line[0] == '#') {
            continue;
        }
        size_t pos = line.find('#');
        if (pos != string::npos) {
            line = line.substr(0, pos);
        }

        istringstream stream(line);
        Cmd cmd;
        stream >> cmd.name;

        string word;
        int arg;
        while (stream >> word) {
            if (word[0] == '$') {
                arg = stoi(word.substr(1));
            } else {
                arg = stoi(word);
            }
            cmd.args.push_back(arg);
        }
        cmds.push_back(cmd);
    }
    fin.close();

    return cmds;
}