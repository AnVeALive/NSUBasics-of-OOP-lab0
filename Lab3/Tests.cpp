#include <iostream>
#include <vector>
#include "Config.h"
#include "Converters.h"
#include "gtest/gtest.h"

using namespace std;

class WavReader {

public:
    void writeInfoFromFirstFile(const string &outFileName, const string &inFileName) {
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
        fout.close();
        fin.close();
    }

    void skipToData(const vector<string> &inputFiles) {

        for (const string &fileName : inputFiles) {
            ifstream fin(fileName);

            if (!fin.is_open()) {
                throw runtime_error("Failed to open file: " + fileName);
            }


        }
    }

    //vector<int16_t> getSampleFromEachFile(const vector<string> &inputFiles)
};

TEST(SoundProcessor, Mute) {
    vector<int16_t> samples;
    for (int i = 0; i < 1000000; i++) {
        samples.push_back(rand() % 100);
    }

    Mute converter(2, 5);
    converter.apply(samples);

    for (int i = 2 * SAMPLE_RATE; i < 5 * SAMPLE_RATE; i++) {
        ASSERT_EQ(samples[i], 0);
    }
}

TEST(SoundProcessor, Reverse) {
    vector<int16_t> samples;
    for (int i = 0; i < 44100; i++) {
        samples.push_back(1);
    }
    for (int i = 0; i < 44100; i++) {
        samples.push_back(0);
    }

    Reverse conv(0, 2);
    conv.apply(samples);

    for (int i = 0; i < 44100; i++) {
        ASSERT_EQ(samples[i], 0);
    }
}

TEST(SoundProcessor, ConfigParser) {
    string fileName = "../config.txt";
    ConfigParser cp;

    Config conf(cp.parse(fileName));

    Cmd cmd = conf.getCmd(1);

    ASSERT_EQ("mix", cmd.name);
    ASSERT_EQ(2, cmd.args[0]);
    ASSERT_EQ(10, cmd.args[1]);
}

TEST(SoundProcessor, WavReader) {
    WavReader wr;

    wr.writeInfoFromFirstFile("../tof.bin", "../funkorama.wav");

}