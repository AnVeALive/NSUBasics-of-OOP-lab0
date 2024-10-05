#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <vector>
#include <algorithm>
#include <iomanip> // for setprecision

using namespace std;

int isSep(char x) {
    if (('a' <= x) && (x <= 'z') ||
        ('A' <= x) && (x <= 'Z') ||
        ('0' <= x) && (x <= '9')) {
        return 0;
    }
    return 1;
}

int isCap(char x) {
    if (('A' <= x) && (x <= 'Z')) {
        return 1;
    }
    return 0;
}

int main(int argc, char **argv) {

    if (argc != 3) {
        cout << "Bad input!" << endl;
    }
    else {
        string pathIn = argv[1];
        string pathOut = argv[2];

        ifstream fin;
        fin.open(pathIn);

        if (!fin.is_open()) {
            cout << "Input file is not opened!" << endl;
        }
        else {
            map<string, int> wordStat;
            int cntWords = 0;

            string line;

            while (!fin.eof()) {

                getline(fin, line);

                string word = "";
                for (int i = 0; i < line.size(); i++) {
                    if (isSep(line[i]) && word.size()) {
                        wordStat[word]++;
                        cntWords++;
                        word = "";
                    }
                    else {
                        if (isCap(line[i]))
                            word += (char)(line[i] + 32);
                        else
                            word += line[i];
                    }
                }
                if (word.size()) {
                    wordStat[word]++;
                    cntWords++;
                    word = "";
                }
            }

            /*for (const auto& pair : wordStat) {
                std::cout << "Key:" << pair.first << " " << pair.second << std::endl;
            } */

            vector<pair<string, int> > vec;

            /* transform(wordStat.begin(), wordStat.end(), back_inserter(vec),
                [](const pair<const string, int>& pair) {
                    return make_pair(pair.first, pair.second);
                }); */
            
            copy(wordStat.begin(), wordStat.end(), back_inserter(vec));
            


            sort(vec.begin(), vec.end(),
                [](const pair<string, int>& a, const pair<string, int>& b) {
                    return a.second > b.second;
                });

            /* for (const auto& pair : vec) {
                cout << pair.first << "_" << pair.second << endl;
            } */

            ofstream fout;
            fout.open(pathOut);

            if (!fout.is_open()) {
                cout << "Output file is not opened!" << endl;
            }
            else {
                fout << "word;frequency;as a percentage\n";

                for (const auto& pair : vec) {
                    fout << pair.first << ';' 
                    << pair.second << ';' 
                    << fixed << setprecision(3)
                    << float(pair.second) / cntWords << '\n';
                }

                fout.close();
            }
        }

        fin.close();
    }

    return 0;
}