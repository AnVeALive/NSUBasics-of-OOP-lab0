#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "SoundProcessor.h"

using namespace std;



int main(int argc, char *argv[]) {

    if (argc < 5) {
        cerr << "There are too few arguments" << endl;
        return 1;
    } 

    if (string(argv[1]) == "-h") {
        cout << "Sound Processor Usage:" << endl;
        cout << "\t-c <config.txt> <output.wav> <input1.wav> [<input2.wav> ...]" << endl;
        cout << "Supported converters:\nmute,\nmix,\nreverse" << endl;
        return 0;
    }
    
    string configFileName(argv[2]);
    string outputFileName(argv[3]);
    vector<string> inputFileNames;

    for (int i = 4; i < argc; i++) {
        inputFileNames.push_back(argv[i]);
    }

    try {
        SoundProcessor processor;
        processor.process(configFileName, outputFileName, inputFileNames);
        cout << "Processing completed successfully." << endl;
    }
    catch (const exception &ex) {
        cerr << "Error: " << ex.what() << endl;
    }

    return 0;
}