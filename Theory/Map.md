```
#include <iostream>
#include <map>
#include <vector>
#include <algorithm>

using namespace std;

int main() {
    
    map<int, int> myMap;
    myMap[1] = 10;
    myMap[2] = 30;
    myMap[3] = 20;
    myMap[4] = 40;

    // Копируем элементы map в вектор пар
    vector<pair<int, int>> vec;
    transform(myMap.begin(), myMap.end(), back_inserter(vec),
              [](const auto& pair) { return make_pair(pair.first, pair.second); });

    return 0;
}
```