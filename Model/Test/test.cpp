#include <iostream>

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

TEST(CatanGraph, ShowGraph) {
    Catan board;
    for (int i = 0; i < FIELDHEIGHT; i ++) {
        for (int j = 0; j < FIELDWIDTH; j ++) {
            if (board.getFieldCell(i, j) != nullptr) {
                std::cout << "X";
            } else {
                std::cout << '.';
            }
        }
        std::cout << '\n';
    }
    std::cout << '\n';

    std::cout << "Vertexes Graph:\n";
    for (int i = 0; i < FIELDHEIGHT; i ++) {
        for (int j = 0; j < FIELDWIDTH; j ++) {
            if (board.getFieldCell(i, j) != nullptr) {
                std::cout << '(' << i << ", " << j << ") : ";
                int lenV = board.getFieldCell(i, j)->getVertexNum();
                for (int v = 0; v < lenV; v++) {
                    std::cout << '(' << board.getFieldCell(i, j)->getVertex(v).first << ", ";
                    std::cout << board.getFieldCell(i, j)->getVertex(v).second << ") ";
                }
            } else {
                continue;
            }
            std::cout << '\n';
        }
    }
    std::cout << '\n';
    std::cout << "Roads Graph:\n";
    for (int i = 0; i < FIELDHEIGHT; i ++) {
        for (int j = 0; j < FIELDWIDTH; j ++) {
            if (board.getFieldCell(i, j) != nullptr) {
                std::cout << '(' << i << ", " << j << ") : ";
                int lenR = board.getFieldCell(i, j)->getRoadsNum();
                for (int v = 0; v < lenR; v++) {
                    std::cout << '(' << board.getFieldCell(i, j)->getRoad(v).first << ", ";
                    std::cout << board.getFieldCell(i, j)->getRoad(v).second << ") ";
                }
            } else {
                continue;
            }
            std::cout << '\n';
        }
    }

    std::cout << '\n';
    std::cout << "Check Hexes Resources\n";
    for (int i = 0; i < HEXESNUM; i++) {
        if (board.getHex(i)->robbersIsHere()) {
            std::cout << 0 << ' ';
            continue;
        }
        std::cout << static_cast<int>(board.getHex(i)->getResource()) << ' ';
    }
    std::cout << '\n';
    std::cout << "Check Hexes Num\n";
    for (int i = 0; i < HEXESNUM; i++) {
        std::cout << board.getHex(i)->getNum() << ' ';
    }
}

TEST(canBuildTest, simpleVillageCase) {
    Catan board;
    board.settle(BuildingType::VILLAGE, 8, 6);
    board.settle(BuildingType::ROAD, 8, 7);
    board.changeCurPlayer(PlayerNum::GAMER2);
    ASSERT_EQ(board.canBuild(BuildingType::VILLAGE, 8, 4), false);
    ASSERT_EQ(board.canBuild(BuildingType::VILLAGE, 8, 2), true);
}

TEST(canBuildTest, nearEnemyVillageCase) {
    Catan board;
    board.settle(BuildingType::VILLAGE, 8, 6);
    board.settle(BuildingType::ROAD, 8, 7);
    board.settle(BuildingType::ROAD, 8, 9);
    board.changeCurPlayer(PlayerNum::GAMER2);
    ASSERT_EQ(board.canBuild(BuildingType::VILLAGE, 8, 10), true);
}

TEST(canBuildTest, afterBeginningVillageCase) {
    Catan board;
    board.settle(BuildingType::VILLAGE, 8, 6);
    board.settle(BuildingType::ROAD, 8, 7);
    board.settle(BuildingType::ROAD, 8, 9);
    board.gotoNextGamePhase();
    ASSERT_EQ(board.canBuild(BuildingType::VILLAGE, 8, 10), true);
    ASSERT_EQ(board.canBuild(BuildingType::VILLAGE, 8, 18), false);
}

TEST(canBuildTest, simpleCityCase) {
    Catan board;
    board.settle(BuildingType::VILLAGE, 8, 6);
    board.changeCurPlayer(PlayerNum::GAMER2);
    board.settle(BuildingType::VILLAGE, 4, 4);
    ASSERT_EQ(board.canBuild(BuildingType::CITY, 8, 6), false);
    ASSERT_EQ(board.canBuild(BuildingType::CITY, 4, 4), true);
}

TEST(canBuildTest, simpleRoadCase) {
    Catan board;
    board.settle(BuildingType::VILLAGE, 8, 6);
    board.settle(BuildingType::ROAD, 8, 7);
    board.changeCurPlayer(PlayerNum::GAMER2);
    board.settle(BuildingType::VILLAGE, 8, 10);
    ASSERT_EQ(board.canBuild(BuildingType::ROAD, 8, 9), true);
    ASSERT_EQ(board.canBuild(BuildingType::ROAD, 8, 7), false);
    ASSERT_EQ(board.canBuild(BuildingType::ROAD, 4, 7), false);
}

TEST(canBuildTest, fromRulesRoadCase) {
    Catan board;
    board.settle(BuildingType::VILLAGE, 8, 6);
    board.settle(BuildingType::ROAD, 9, 6);
    board.changeCurPlayer(PlayerNum::GAMER2);
    board.settle(BuildingType::VILLAGE, 8, 10);
    board.settle(BuildingType::ROAD, 8, 9);
    ASSERT_EQ(board.canBuild(BuildingType::ROAD, 8, 7), true);
    board.settle(BuildingType::ROAD, 8, 7);
    ASSERT_EQ(board.canBuild(BuildingType::ROAD, 9, 8), true);
    ASSERT_EQ(board.canBuild(BuildingType::ROAD, 7, 10), true);
    ASSERT_EQ(board.canBuild(BuildingType::ROAD, 8, 11), true);
    ASSERT_EQ(board.canBuild(BuildingType::ROAD, 9, 8), true);
    ASSERT_EQ(board.canBuild(BuildingType::ROAD, 8, 5), false);
}
