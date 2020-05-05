#include "time.h"
#include "gtest/gtest.h"
#include "../Utility/random.h"

std::mt19937 utility::Random::random_;

void seedRandom() {
    std::random_device rd;
    unsigned int x = rd();
    utility::Random::random_.seed(x);
}

int main(int argc, char **argv) {
    seedRandom();
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}