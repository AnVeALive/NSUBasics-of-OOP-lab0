#include <string>
#include <vector>
#include "CommandStructure.h"

using namespace std;



class SoundProcessor {
private:
    bool isNeedToApplyConverter(const Cmd &cmd, const int numSample);

public:
    void process(const string &configFileName, const string &outputFileName,
    const vector<string> &inputFileNames);
};