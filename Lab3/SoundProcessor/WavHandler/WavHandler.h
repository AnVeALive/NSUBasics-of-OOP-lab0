#pragma once
#include <fstream>
#include <string>
#include <vector>

using namespace std;



class WavHandler {
public:
    int  getDataSize(const string &fileName);

    void skipBytes(ifstream &fin, const int bytesCount);
    void skipToData(ifstream &fin);

    void getNextSamples(vector<int16_t*> &samples, vector<ifstream> &inputFiles);

    void write(ofstream &fout, int16_t &sample);
    void writeInfoFromFirstFile(const string &outFileName, const string &inFileName);
};