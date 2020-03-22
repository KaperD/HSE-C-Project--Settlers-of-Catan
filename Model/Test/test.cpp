#include <iostream>

#include "gtest/gtest.h"
#include "../Board.h"

using namespace Board;

TEST(HexagonNum, GetRandomNum) {
    Hexagon hex(1, 6);
    ASSERT_EQ(hex.robbersIsHere(), false);
}

TEST(HexagonNum, GetRandomNum2) {
    Hexagon hex(1, 6);
    hex.setRobbers();
    ASSERT_EQ(hex.robbersIsHere(), true);
}

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
}


