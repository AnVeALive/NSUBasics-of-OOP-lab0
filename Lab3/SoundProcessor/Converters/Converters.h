#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "../Config/Config.h"

#define SAMPLE_RATE 44100
#define BITS_PER_SAMPLE 16



class Converter {
public:
    virtual ~Converter();
    virtual void apply(std::vector<int16_t*> &samples) = 0;
    virtual bool isApplicable(const int numSample);
};



class Mute : public Converter {
private:
    int _begin;
    int _end;

public:
    Mute(const int begin, const int end);
    ~Mute() override;
    void apply(std::vector<int16_t*> &samples) override;
    bool isApplicable(const int numSample) override;
};



class Mix : public Converter {
private:
    int _numAddStream;
    int _begin;

public:
    Mix(const int numAddStream, const int begin);
    ~Mix() override;
    void apply(std::vector<int16_t*> &samples) override;
    bool isApplicable(const int numSample) override;
};



class ConverterFactory {
public:
    Converter *create(const Cmd &cmd);
};