#include "WavHandler.h"

using namespace std;



int WavHandler::getDataSize(const string &fileName) {
    ifstream fin(fileName, ios::binary);

    if (!fin.is_open()) {
        throw runtime_error("Failed to Open File: " + fileName);
    }
    skipToData(fin);
    skipBytes(fin, 4);

    int size;
    fin.read(reinterpret_cast<char*>(&size), sizeof(int));

    fin.close();

    return size;
}

void WavHandler::skipBytes(ifstream &fin, const int bytesCount) {
    fin.seekg(bytesCount, ios::cur);
}

void WavHandler::skipToData(ifstream &fin) {
    fin.seekg(36, ios::beg);

    char word[5];
    fin.read(word, 4);
    if (string(word) == "LIST") {
        int size;
        fin.read(reinterpret_cast<char*>(&size), sizeof(int));
        fin.seekg(size, ios::cur);
    }
}

// ! <int16_t*>
void WavHandler::getNextSamples(vector<int16_t*> &samples, vector<ifstream> &inputFiles) {
    for (int i = 0; i < samples.size(); i++) {
        int16_t sample;
        if (!inputFiles[i].read(reinterpret_cast<char*>(&sample), sizeof(int16_t))) {
            if (inputFiles[i].eof() && samples[i] != nullptr) {
                delete samples[i];
                samples[i] = nullptr;
            }
        } else {
            *samples[i] = sample;
        }
    }
}

void WavHandler::write(ofstream &fout, int16_t &sample) {
    fout.write(reinterpret_cast<char*>(&sample), 2);
}

void WavHandler::writeInfoFromFirstFile(const string &outFileName, const string &inFileName) {
    ofstream fout(outFileName, ios::binary);

    if (!fout.is_open()) {
        throw runtime_error("Failed to Open File: " + outFileName);
    }

    ifstream fin(inFileName, ios::binary);

    if (!fin.is_open()) {
        throw runtime_error("Failed to Open File: " + inFileName);
    }

    char byte;
    for (int i = 0; i < 36; i++) {
        fin.read(&byte, sizeof(char));
        fout.write(&byte, sizeof(char));
    }
    
    char word[5];
    int size;
    fin.read(word, 4);
    if (string(word) == "LIST") {
        fin.read(reinterpret_cast<char*>(&size), sizeof(size));
        fin.seekg(size, ios::cur);
    }
    fin.read(word, 4);
    fout.write(word, 4);
    fin.read(reinterpret_cast<char*>(&size), sizeof(int));
    fout.write(reinterpret_cast<char*>(&size), sizeof(int));

    fout.close();
    fin.close();
}