#include "SoundProcessor.h"
#include "Config.h"
#include "Converters.h"
#include "WavHandler.h"
#include <cstdint>



void SoundProcessor::process(const string &configFileName, const string &outputFileName, const vector<string> &inputFileNames) {
    // Создание конфига.
    ConfigParser cp;
    Config config(cp.parse(configFileName));

    // Создание вектора конверторов на основе config.
    vector<Converter*> converters;
    ConverterFactory factory;
    for (int i = 0; i < config.getSize(); i++) {
        Converter *converter = factory.create(config.getCmd(i));
        converters.push_back(converter);
    }

    // Запись вводной информации о первом входном файле.
    WavHandler wh;
    wh.writeInfoFromFirstFile(outputFileName, inputFileNames[0]);

    // Открытие файла для вывода в режиме дополнение.
    ofstream fout(outputFileName, ios::binary | ios::app);

    if (!fout.is_open()) {
        throw runtime_error("Failed to Open File: " + outputFileName);
    }

    // Получение кол-ва сэмплов в первом входном файле.
    int dataSize = wh.getDataSize(inputFileNames[0]) / 2;

    // Открытие всех входных файлов и переход к сэмплам у каждого из них.
    vector<ifstream> inputFiles;
    for (const auto &fileName : inputFileNames) {
        inputFiles.emplace_back(fileName);

        if (!inputFiles.back().is_open()) {
            throw runtime_error("Failed to Open File: " + fileName);
        }
        wh.skipToData(inputFiles.back());
        wh.skipBytes(inputFiles.back(), 8);
    }

    // Считывание по одному сэмплу из каждого файла и
    // запись результата в выходной файл.
    vector<int16_t*> samples(inputFileNames.size());
    for (int i = 0; i < inputFileNames.size(); i++) {
        samples[i] = new int16_t(0);
    }
    for (int i = 1; i <= dataSize; i++) {
        wh.getNextSamples(samples, inputFiles);

        for (int indCmd = 0; indCmd < config.getSize(); indCmd++) {
            if (converters[indCmd]->isApplicable(i)) {
                converters[indCmd]->apply(samples);
            }
        }
        wh.write(fout, *samples[0]);
    }

    for (int i = 0; i < inputFileNames.size(); i++) {
        delete samples[i];
    }

    // Clearning vector of converters
    for (Converter* &converter : converters) {
        delete converter;
    }
    converters.clear();

    // Closing all input files
    for (auto &file : inputFiles) {
        if (file.is_open()) {
            file.close();
        }
    }
    fout.close();
}