#include "gtest/gtest.h"
#include "../Board.h"

using namespace Board;

TEST(CatanGraph, HexDesert) {
    Catan board;
    int desert_cnt = 0;
    for (int i = 0; i < HEXESNUM; i++) {
        if (board.getHex(i)->robbersIsHere()) {
            desert_cnt++;
        }
    }
    ASSERT_EQ(desert_cnt, 1);
}