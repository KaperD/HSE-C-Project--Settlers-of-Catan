#pragma once

#include <random>

namespace utility {

class Random {
public:
    static int getRandomNumberFromTo(int from, int to);
    static std::mt19937 random_;
};

} // namespace utility