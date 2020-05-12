#include <iostream>
#include <unordered_map>

#include "gtest/gtest.h"
#include "../Board.h"

using utility::Random;
using namespace Board;


TEST(CatanGraph, ShowGraph) {
    Random random;
    Catan board(random, 4);
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

    std::cout << "\nCheck Hexes Vertices\n";
    for (int i = 0; i < HEXESNUM; i++) {
        std::cout << i << ": ";
        int lenV = board.getHex(i)->getVertexNum();
        for (int v = 0; v < lenV; v++) {
            std::cout << '(' << board.getHex(i)->getVertex(v).first << ", ";
            std::cout << board.getHex(i)->getVertex(v).second << ") ";
        }
        std::cout << '\n';
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
