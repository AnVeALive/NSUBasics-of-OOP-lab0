#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "../Config/Config.h"

#define SAMPLE_RATE 44100
#define BITS_PER_SAMPLE 16



class Converter {
public:
    virtual void apply(std::vector<int16_t> &samples) = 0;
    virtual ~Converter();
};



class Mute : public Converter {
public:
    ~Mute() override;
    void apply(std::vector<int16_t> &samples) override;
};



class Mix : public Converter {
private:
    int _numAddStream;

public:
    Mix(const int numAddStream);
    ~Mix() override;
    void apply(std::vector<int16_t> &samples) override;
};



class ConverterFactory {
public:
    Converter *create(const Cmd &cmd);
};