#include <iostream>
#include <fstream>
#include "TupleTools.h"
#include "CsvParser.h"

#define TableTemplate int, std::string, float, int



int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Too few arguments!" << std::endl;
        return 1;
    }

    std::ifstream file(argv[1]);
    if (!file.is_open()) {
        std::cerr << "File is not opened!" << std::endl;
        return 1;
    }

    try {
        CsvParser<TableTemplate> parser(file, 0);
        for (auto rs : parser) {
            std::cout << '(' << rs << ')' << std::endl;
        }
    } catch (const std::exception& ex) {
        std::cerr << ex.what() << std::endl;
    } catch (...) {
        std::cerr << "Unknown Errow." << std::endl;
    }

    return 0;
}