#include "Converters.h"



Converter::~Converter() {}

void Converter::apply(std::vector<int16_t> &samples) {}



Mute::~Mute() {}

void Mute::apply(std::vector<int16_t> &samples) {
    samples[0] = 0;
}



Mix::Mix(const int numAddStream) {
    _numAddStream = numAddStream;
}

Mix::~Mix() {}

void Mix::apply(std::vector<int16_t> &samples) {
    samples[0] = (samples[_numAddStream - 1] + samples[0]) / 2;
}



Converter *ConverterFactory::create(const Cmd &cmd) {
    if (cmd.name == "mute") {
        return new Mute();
    } else if (cmd.name == "mix") {
        return new Mix(cmd.args[0]);
    } else {
        throw std::runtime_error("There is no so Converter!");
    }
}