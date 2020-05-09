#include "gtest/gtest.h"
#include "../Board.h"
#include "../Utility/random.h"

using utility::Random;
using namespace Board;

TEST(CatanGraph, HexDesert) {
    Random random;
    Catan board(random, 4);
    int desert_cnt = 0;
    for (int i = 0; i < HEXESNUM; i++) {
        if (board.getHex(i)->robbersIsHere()) {
            desert_cnt++;
        }
    }
    ASSERT_EQ(desert_cnt, 1);
}

//TODO: giveResources tests

//TODO: setRobbers tests