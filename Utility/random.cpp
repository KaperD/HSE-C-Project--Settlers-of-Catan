#include "random.h"
#include <fstream>
#include <vector>
#include <algorithm>

int utility::Random::getRandomNumberFromTo(int from, int to) {
    std::uniform_int_distribution<int> cut(from, to);
    return cut(random_);
}

utility::Random::Random() : cells(19) {
    std::random_device rd;
    unsigned int x = rd();
    random_.seed(x);
    std::ofstream fout("RandomLog.txt", std::ios_base::app);
    fout << x << '\n';

    std::vector<int> numbers = {2, 3, 3, 4, 4, 5, 5, 6, 6, 8, 8, 9, 9, 10, 10, 11, 11, 12};
    std::shuffle(numbers.begin(), numbers.end(), std::mt19937(std::random_device()()));

    std::vector<int> resources = {0,  1, 1, 1, 1,  2, 2, 2,  3, 3, 3,  4, 4, 4, 4,  5, 5, 5, 5};
    std::shuffle(resources.begin(), resources.end(), std::mt19937(std::random_device()()));

    int posNumbers = 0;
    for (int k = 0; k < 19; ++k) {
        cells[k].resource = resources[k];
        if (cells[k].resource == 0) {
            cells[k].number = 0;
            continue;
        }
        cells[k].number = numbers[posNumbers];
        ++posNumbers;
    }
}

std::vector<utility::cellInfo>& utility::Random::generateResourcesAndNumbers() {
    return cells;
}
