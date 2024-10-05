```
#include <iostream>
#include <string>
#include <fstream>
#include <map>
using namespace std;

int main(int argc, char **argv) {

    //string pathIn = "input.txt";
    //string pathOut = "output.txt";

    ifstream fin;
    fin.open("input.txt");

    if (!fin.is_open()) {
        cout << "File is not opened!" << endl;
    }
    else {
        cout << "File is opened)\n" << endl;

        string str;

        while (!fin.eof()) {
            //fin >> str;
            //cout << str << endl;

            getline(fin, str);
            cout << str << endl;
        }
    }

    fin.close();

    return 0;
}
```