#include "random.h"
#include <fstream>

utility::Random::Random() {
    std::random_device rd;
    int x = rd();
    random_.seed(x);
    std::ofstream fout("RandomLog.txt", std::ios_base::app);
    fout << x << 'n';
}

int utility::Random::getRandomNumberFromTo(int from, int to) {
    std::uniform_int_distribution<int> cut(from, to);
    return cut(random_);
}
