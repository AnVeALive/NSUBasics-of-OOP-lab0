#include "Converters.h"



Converter::~Converter() {}

void Converter::apply(std::vector<int16_t*> &samples) {}

bool Converter::isApplicable(const int numSample) {
    return true;
}



Mute::Mute(const int begin, const int end) : _begin(begin), _end(end) {}

Mute::~Mute() {}

void Mute::apply(std::vector<int16_t*> &samples) {
    *samples[0] = 0;
}

bool Mute::isApplicable(const int numSample) {
    if (_begin * SAMPLE_RATE < numSample && numSample <= _end * SAMPLE_RATE) {
        return true;
    } else {
        return false;
    }
}



Mix::Mix(const int numAddStream, const int begin) :
_numAddStream(numAddStream), _begin(begin) {}

Mix::~Mix() {}

// Поблема: если второй файл короче => то после длины второго файла
// сэмплы первоначального будут делиться на 2
void Mix::apply(std::vector<int16_t*> &samples) {
    if (samples[_numAddStream - 1] != nullptr) {
        *samples[0] = (*samples[_numAddStream - 1] + *samples[0]) / 2;
    }
}

bool Mix::isApplicable(const int numSample) {
    if (_begin * SAMPLE_RATE < numSample) {
        return true;
    } else {
        return false;
    }
}



Converter *ConverterFactory::create(const Cmd &cmd) {
    if (cmd.name == "mute") {
        return new Mute(cmd.args[0], cmd.args[1]);
    } else if (cmd.name == "mix") {
        return new Mix(cmd.args[0], cmd.args[1]);
    } else {
        throw std::runtime_error("There is no so Converter!");
    }
}