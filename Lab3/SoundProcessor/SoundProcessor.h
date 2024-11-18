#include <string>
#include <vector>
#include "CommandStructure.h"

using namespace std;



class SoundProcessor {
public:
    void process(const string &configFileName, const string &outputFileName,
    const vector<string> &inputFileNames);
};