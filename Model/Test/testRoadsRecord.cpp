#include "gtest/gtest.h"
#include "../Board.h"

using utility::Random;
using namespace Board;

TEST(RoadsRecord, simple_case) {
    Random random;
    Catan board(random, 2);
    board.settle(BuildingType::VILLAGE, 0, 4);
    board.settle(BuildingType::ROAD, 1, 4);
    board.settle(BuildingType::ROAD, 2, 5);
    board.settle(BuildingType::ROAD, 2, 7);
    board.settle(BuildingType::ROAD, 2, 9);
    board.settle(BuildingType::ROAD, 3, 10);
    ASSERT_EQ(board.getRoadsRecordHolder(), PlayerNum::GAMER1);
    ASSERT_EQ(board.getRoadsRecord(), 5);
}

TEST(RoadsRecord, record_race) {
    Random random;
    Catan board(random, 2);
    board.settle(BuildingType::VILLAGE, 0, 4);
    board.settle(BuildingType::ROAD, 1, 4);
    board.settle(BuildingType::ROAD, 2, 5);
    board.settle(BuildingType::ROAD, 2, 7);
    board.settle(BuildingType::ROAD, 2, 9);
    board.settle(BuildingType::ROAD, 3, 10);

    board.changeCurPlayer(PlayerNum::GAMER2);
    board.settle(BuildingType::VILLAGE, 6, 8);
    board.settle(BuildingType::ROAD, 6, 9);
    board.settle(BuildingType::ROAD, 6, 11);
    board.settle(BuildingType::ROAD, 5, 12);
    board.settle(BuildingType::ROAD, 4, 13);
    board.settle(BuildingType::ROAD, 4, 15);
    board.settle(BuildingType::ROAD, 5, 16);

    ASSERT_EQ(board.getRoadsRecordHolder(), PlayerNum::GAMER2);
    ASSERT_EQ(board.getRoadsRecord(), 6);
}

TEST(RoadsRecord, enemy_village) {
    Random random;
    Catan board(random, 2);
    board.settle(BuildingType::VILLAGE, 4, 2);
    board.settle(BuildingType::ROAD, 4, 3);
    board.settle(BuildingType::ROAD, 4, 5);

    board.changeCurPlayer(PlayerNum::GAMER2);
    board.settle(BuildingType::VILLAGE, 4, 6);

    board.changeCurPlayer(PlayerNum::GAMER1);
    board.settle(BuildingType::ROAD, 4, 7);
    board.settle(BuildingType::ROAD, 4, 8);
    board.settle(BuildingType::ROAD, 4, 9);

    ASSERT_EQ(board.getRoadsRecordHolder(), PlayerNum::NONE);
}

TEST(RoadsRecord, from_rules) {
    Random random;
    Catan board(random, 2);
    board.settle(BuildingType::VILLAGE, 4, 2);
    board.settle(BuildingType::ROAD, 4, 1);
    board.settle(BuildingType::ROAD, 4, 3);
    board.settle(BuildingType::ROAD, 5, 4);
    board.settle(BuildingType::ROAD, 6, 5);
    board.settle(BuildingType::ROAD, 6, 7);

    ASSERT_EQ(board.getRoadsRecordHolder(), PlayerNum::GAMER1);
    ASSERT_EQ(board.getRoadsRecord(), 5);

    board.changeCurPlayer(PlayerNum::GAMER2);
    board.settle(BuildingType::VILLAGE, 6, 8);
    board.settle(BuildingType::ROAD, 6, 9);
    board.settle(BuildingType::ROAD, 6, 11);
    board.settle(BuildingType::VILLAGE, 6, 12);
    board.settle(BuildingType::ROAD, 6, 13);
    board.settle(BuildingType::ROAD, 6, 15);
    board.settle(BuildingType::ROAD, 5, 12);
    board.settle(BuildingType::ROAD, 4, 13);
    board.settle(BuildingType::ROAD, 4, 15);
    // board.settle(BuildingType::ROAD, 5, 16); так получаем цикл и ответ уже 8
    board.settle(BuildingType::ROAD, 4, 17);

    ASSERT_EQ(board.getRoadsRecordHolder(), PlayerNum::GAMER2);
    ASSERT_EQ(board.getRoadsRecord(), 6);
}

TEST(RoadsRecord, branches) {
    Random random;
    Catan board(random, 2);

    board.settle(BuildingType::VILLAGE, 4, 10);
    board.settle(BuildingType::ROAD, 4, 9);
    board.settle(BuildingType::ROAD, 4, 7);
    board.settle(BuildingType::ROAD, 5, 8);
    board.settle(BuildingType::ROAD, 3, 10);
    board.settle(BuildingType::ROAD, 4, 11);
    board.settle(BuildingType::ROAD, 4, 13);

    ASSERT_EQ(board.getRoadsRecordHolder(), PlayerNum::NONE);
}

TEST(RoadsRecord, fork) {
    Random random;
    Catan board(random, 2);

    board.settle(BuildingType::VILLAGE, 2, 10);
    board.settle(BuildingType::ROAD, 3, 10);
    board.settle(BuildingType::ROAD, 4, 9);
    board.settle(BuildingType::ROAD, 4, 7);
    board.settle(BuildingType::ROAD, 4, 5);
    board.settle(BuildingType::ROAD, 4, 11);
    board.settle(BuildingType::ROAD, 4, 13);
    board.settle(BuildingType::ROAD, 4, 15);

    ASSERT_EQ(board.getRoadsRecordHolder(), PlayerNum::GAMER1);
    ASSERT_EQ(board.getRoadsRecord(), 6);
}

TEST(RoadsRecord, cycle_level0) {
    Random random;
    Catan board(random, 2);
    board.settle(BuildingType::VILLAGE, 6 ,8);
    board.settle(BuildingType::ROAD, 6, 9);
    board.settle(BuildingType::ROAD, 6, 7);
    board.settle(BuildingType::ROAD, 7, 6);
    board.settle(BuildingType::ROAD, 7, 10);
    board.settle(BuildingType::ROAD, 8, 7);
    board.settle(BuildingType::ROAD, 8, 9);

    ASSERT_EQ(board.getRoadsRecordHolder(), PlayerNum::GAMER1);
    ASSERT_EQ(board.getRoadsRecord(), 6);
}

TEST(RoadsRecord, cycle_level1) {
    Random random;
    Catan board(random, 2);

    board.settle(BuildingType::VILLAGE, 6 ,4);
    board.settle(BuildingType::ROAD, 6, 5);
    board.settle(BuildingType::ROAD, 6, 7);
    board.settle(BuildingType::ROAD, 6, 9);
    board.settle(BuildingType::ROAD, 6, 11);
    board.settle(BuildingType::ROAD, 7, 6);
    board.settle(BuildingType::ROAD, 7, 10);
    board.settle(BuildingType::ROAD, 8, 7);
    board.settle(BuildingType::ROAD, 8, 9);

    ASSERT_EQ(board.getRoadsRecordHolder(), PlayerNum::GAMER1);
    ASSERT_EQ(board.getRoadsRecord(), 7);
}

TEST(RoadsRecord, cycle_level2) {
    Random random;
    Catan board(random, 2);

    board.settle(BuildingType::VILLAGE, 6 ,8);
    board.settle(BuildingType::ROAD, 6, 7);
    board.settle(BuildingType::ROAD, 6, 9);
    board.settle(BuildingType::ROAD, 6, 5);
    board.settle(BuildingType::ROAD, 6, 11);
    board.settle(BuildingType::ROAD, 7, 6);
    board.settle(BuildingType::ROAD, 7, 10);
    board.settle(BuildingType::ROAD, 8, 7);
    board.settle(BuildingType::ROAD, 8, 9);

    ASSERT_EQ(board.getRoadsRecordHolder(), PlayerNum::GAMER1);
    ASSERT_EQ(board.getRoadsRecord(), 7);
}

TEST(RoadsRecord, cycle_level3_boss) {
    Random random;
    Catan board(random, 2);
    board.settle(BuildingType::VILLAGE, 4 ,8);
    board.settle(BuildingType::ROAD, 4, 7);
    board.settle(BuildingType::ROAD, 4, 9);
    board.settle(BuildingType::ROAD, 4, 5);
    board.settle(BuildingType::ROAD, 4, 11);
    board.settle(BuildingType::ROAD, 3, 6);
    board.settle(BuildingType::ROAD, 3, 10);
    board.settle(BuildingType::ROAD, 2, 7);
    board.settle(BuildingType::ROAD, 2, 9);

    board.settle(BuildingType::ROAD, 5, 8);
    board.settle(BuildingType::ROAD, 5, 4);
    board.settle(BuildingType::ROAD, 5, 12);

    board.settle(BuildingType::ROAD, 6, 7);
    board.settle(BuildingType::ROAD, 6, 9);
    board.settle(BuildingType::ROAD, 6, 5);
    board.settle(BuildingType::ROAD, 6, 11);
    board.settle(BuildingType::ROAD, 7, 6);
    board.settle(BuildingType::ROAD, 7, 10);
    board.settle(BuildingType::ROAD, 8, 7);
    board.settle(BuildingType::ROAD, 8, 9);

    ASSERT_EQ(board.getRoadsRecordHolder(), PlayerNum::GAMER1);
    ASSERT_EQ(board.getRoadsRecord(), 16);
}

TEST(RoadsRecord, cycle_level_death) { //СМЕРТЬ
    Random random;
    Catan board(random, 2);
    board.settle(BuildingType::VILLAGE, 4 ,8);
    board.settle(BuildingType::ROAD, 5, 8);
    board.settle(BuildingType::ROAD, 6, 9);
    board.settle(BuildingType::ROAD, 6, 11);
    //board.settle(BuildingType::ROAD, 5, 12);
    board.settle(BuildingType::ROAD, 4, 9);
    board.settle(BuildingType::ROAD, 4, 11);
    board.settle(BuildingType::ROAD, 4, 13);
    board.settle(BuildingType::ROAD, 4, 15);

    board.settle(BuildingType::ROAD, 5, 16);
    board.settle(BuildingType::ROAD, 6, 13);
    board.settle(BuildingType::ROAD, 6, 15);

    ASSERT_EQ(board.getRoadsRecordHolder(), PlayerNum::GAMER1);
    ASSERT_EQ(board.getRoadsRecord(), 10);
}

TEST(RoadsRecord, dynamic) {
    Random random;
    Catan board(random, 2);

    board.settle(BuildingType::VILLAGE, 0, 4);
    board.settle(BuildingType::ROAD, 1, 4);
    board.settle(BuildingType::ROAD, 2, 5);
    board.settle(BuildingType::ROAD, 2, 7);
    board.settle(BuildingType::ROAD, 2, 9);
    board.settle(BuildingType::ROAD, 3, 10);

    board.changeCurPlayer(PlayerNum::GAMER2);
    board.settle(BuildingType::VILLAGE, 4, 10);
    board.settle(BuildingType::ROAD, 4, 9);
    board.settle(BuildingType::ROAD, 4, 7);
    board.settle(BuildingType::ROAD, 4, 5);
    board.settle(BuildingType::ROAD, 4, 3);
    board.settle(BuildingType::ROAD, 4, 11);
    board.settle(BuildingType::ROAD, 4, 13);

    ASSERT_EQ(board.getRoadsRecordHolder(), PlayerNum::GAMER2);
    ASSERT_EQ(board.getRoadsRecord(), 6);
}
