#pragma once

#include <random>
#include <vector>

namespace utility {

struct cellInfo {
    int resource = 0;
    int number = 0;
};

class Random {
public:
    explicit Random(unsigned int seed);
    Random(const Random&) = delete;
    Random& operator=(const Random&) = delete;

    int getRandomNumberFromTo(int from, int to);

    std::vector<cellInfo>& generateResourcesAndNumbers();

private:
    std::mt19937 random_;
    std::vector<cellInfo> cells;
};

} // namespace utility