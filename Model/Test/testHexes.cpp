#include "gtest/gtest.h"
#include "../Board.h"

using utility::Random;
using namespace Board;

TEST(HexesTest, HexDesert) {
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

TEST(HexesTest, giveResourcesTest) {
    Random random;
    Catan board(random, 2);
    board.settle(BuildingType::VILLAGE, 6, 0);
    board.settle(BuildingType::ROAD, 5, 0);
    board.settle(BuildingType::VILLAGE, 6, 18);
    board.settle(BuildingType::ROAD, 6, 19);

    board.changeCurPlayer(PlayerNum::GAMER2);
    board.settle(BuildingType::VILLAGE, 6, 4);
    board.settle(BuildingType::ROAD, 2, 9);
    board.settle(BuildingType::VILLAGE, 4, 16);
    board.settle(BuildingType::ROAD, 5, 16);

    std::vector<Resource> res = {
            Resource::NONE, Resource::WHEAT,
            Resource::WOOL, Resource::CLAY,
            Resource::TREE, Resource::ORE
    };

    for (auto re : res) {
        ASSERT_EQ(board.getPlayerResNum(PlayerNum::GAMER1, re), 0);
        ASSERT_EQ(board.getPlayerResNum(PlayerNum::GAMER2, re), 0);
    }

    int hexNum = 14;
    if (board.getHex(14)->robbersIsHere()) hexNum = 18;

    board.giveResources(board.getHex(hexNum)->getNum());
    ASSERT_EQ(board.getPlayerResNum(PlayerNum::GAMER2, board.getHex(hexNum)->getResource()), 1);
    ASSERT_EQ(board.getPlayerResNum(PlayerNum::GAMER1, board.getHex(hexNum)->getResource()), 1);
}

TEST(HexesTest, setRobbersTest) {
    Random random;
    Catan board(random, 2);
    board.settle(BuildingType::VILLAGE, 2, 10);
    board.settle(BuildingType::ROAD, 2, 11);
    board.settle(BuildingType::VILLAGE, 8, 8);
    board.settle(BuildingType::ROAD, 8, 9);

    board.changeCurPlayer(PlayerNum::GAMER2);
    board.settle(BuildingType::VILLAGE, 6, 4);
    board.settle(BuildingType::ROAD, 2, 9);
    board.settle(BuildingType::VILLAGE, 4, 16);
    board.settle(BuildingType::ROAD, 5, 16);

    std::vector<Resource> res = {
            Resource::NONE, Resource::WHEAT,
            Resource::WOOL, Resource::CLAY,
            Resource::TREE, Resource::ORE
    };

    for (auto re : res) {
        ASSERT_EQ(board.getPlayerResNum(PlayerNum::GAMER1, re), 0);
        ASSERT_EQ(board.getPlayerResNum(PlayerNum::GAMER2, re), 0);
    }

    int hexNum = 8;
    if (board.getHex(hexNum)->robbersIsHere()) hexNum = 9;

    board.giveResources(board.getHex(hexNum)->getNum());
    Resource hexRe = board.getHex(hexNum)->getResource();

    ASSERT_EQ(board.getPlayerResNum(PlayerNum::GAMER1, hexRe), 1);
    ASSERT_EQ(board.getPlayerResNum(PlayerNum::GAMER2, hexRe), 0);

    board.changeCurPlayer(PlayerNum::GAMER2);
    board.setRobbers(hexNum);
    ASSERT_EQ(board.getRobbersIndx(), hexNum);
    ASSERT_EQ(board.getPlayerResNum(PlayerNum::GAMER1, hexRe), 0);
    ASSERT_EQ(board.getPlayerResNum(PlayerNum::GAMER2, hexRe), 1);
}