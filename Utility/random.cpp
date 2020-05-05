#include "random.h"
#include <fstream>

int utility::Random::getRandomNumberFromTo(int from, int to) {
    std::uniform_int_distribution<int> cut(from, to);
    return cut(random_);
}
