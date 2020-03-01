#include <cstdlib>
#include <ctime>
#include <iostream> //for debug

#include "Board.h"

#define getCellPlayer(v) board.getFieldCell(v.first, v.second).getPlayer()

static bool check(int x, int y) {
    return (x < 0 || y < 0 || x >= 11 || y >= 21  ||
          ((x < 2 || x > 8) && (y < 4 || y > 16)) ||
          ((x < 4 || x > 6) && (y < 2 || y > 18)));
}

Player::Player(PlayerNum id) : id(id) {}

PlayerNum Cell::getPlayer() const {
    return player;
}

void Cell::setPlayer(PlayerNum new_player) {
    player = new_player;
}

Vertex::Vertex(int x, int y, bool direction) : Cell() {
    if (direction) {
        if (!check(x - 2, y)) {
            vertexes.push_back({x - 2, y});
        }
        if (!check(x - 1, y)) {
            roads.push_back({x - 1, y});
        }
    } else {
        if (!check(x + 2, y)) {
            vertexes.push_back({x + 2, y});
        }
        if (!check(x + 1, y)) {
            roads.push_back({x + 1, y});
        }
    }
    if (!check(x, y + 2)) {
        vertexes.push_back({x, y + 2});
    }
    if (!check(x, y - 2)) {
        vertexes.push_back({x, y - 2});
    }

    if (!check(x, y + 1)) {
        roads.push_back({x, y + 1});
    }
    if (!check(x, y - 1)) {
        roads.push_back({x, y - 1});
    }
}

Settlement Vertex::getSettlement() const {
    return s;
}

void Vertex::setSettlement(Settlement new_s) {
    s = new_s;
}

bool Vertex::canBuild(PlayerNum cur_player) const {
    if (player != cur_player) {
        return false;
    }
    /*for (auto v : vertexes) {
        if (getCellPlayer(v) != PlayerNum::NONE) {
            return false;
        }
    }*/
    return true;
}

Road::Road(int x, int y, bool is_horizontal, bool is_even) : Cell() {
    //тут много кода повторяется, потом поправлю
    if (is_horizontal) {
        //vertexes
        vertexes.push_back({x, y - 1});
        vertexes.push_back({x, y + 1});
        //roads
        if (!check(x, y + 2)) {
            roads.push_back({x, y + 2});
        }
        if (!check(x, y - 2)) {
            roads.push_back({x, y - 2});
        }
        if (is_even) {
            if (!check(x + 1, y + 1)) {
                roads.push_back({x + 1, y + 1});
            }
            if (!check(x - 1, y - 1)) {
                roads.push_back({x - 1, y - 1});
            }
        } else {
            if (!check(x - 1, y + 1)) {
                roads.push_back({x - 1, y + 1});
            }
            if (!check(x + 1, y - 1)) {
                roads.push_back({x + 1, y - 1});
            }
        }
    } else {
        //vertexes
        vertexes.push_back({x + 1, y});
        vertexes.push_back({x - 1, y});
        //roads
        if (!check(x - 1, y + 1)) {
            roads.push_back({x - 1, y + 1});
        }
        if (!check(x + 1, y - 1)) {
            roads.push_back({x + 1, y - 1});
        }
        if (!check(x + 1, y + 1)) {
            roads.push_back({x + 1, y + 1});
        }
        if (!check(x - 1, y - 1)) {
            roads.push_back({x - 1, y - 1});
        }
    }
}

bool Road::canBuild(PlayerNum cur_player) const {
    if (player != cur_player) {
        return false;
    }
    /*for (auto v : vertexes) {
        if (getCellPlayer(v) == cur_player) {
            return true;
        }
    }
    for (auto road : roads) {
        if (getCellPlayer(road) == cur_player) {
            return true;
        }
    }*/
    return false;
}

Hexagon::Hexagon(int x, int y) : Cell() {
    vertexes.push_back({x + 1, y + 2});
    vertexes.push_back({x + 1, y});
    vertexes.push_back({x + 1, y - 2});
    vertexes.push_back({x - 1, y + 2});
    vertexes.push_back({x - 1, y});
    vertexes.push_back({x - 1, y - 2});

    int random_resource = rand() % TERRITORIESNUM + 1;
    re = static_cast<Resource>(random_resource);
    robbers = false;
    num = rand() % 12 + 1;
    if (num == 1) num++;
}

void Hexagon::setRobbers() {
    robbers = true;
}

Resource Hexagon::getResource() const {
    return re;
}

int Hexagon::getNum() const {
    return num;
}

bool Hexagon::canBuild(PlayerNum cur_player) const {
    return false;
}

Catan::Catan() : field(11, std::vector<Cell*>(21)) {
    srand(time (NULL));

    players.push_back(new Player(PlayerNum::GAMER1));
    players.push_back(new Player(PlayerNum::GAMER2));
    players.push_back(new Player(PlayerNum::GAMER3));
    cur_player = PlayerNum::GAMER1;

    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 4; j++) {
            field[i][j] = nullptr;
            field[i][20 - j] = nullptr;
            field[10 - i][j] = nullptr;
            field[10 - i][20 - j] = nullptr;
        }
    }
    for (int i = 2; i < 4; i++) {
        for (int j = 0; j < 2; j++) {
            field[i][j] = nullptr;
            field[10 - i][j] = nullptr;
            field[i][20 - j] = nullptr;
            field[10 - i][20 - j] = nullptr;
        }
    }

    for (int i = 1; i < 4; i += 2) {
        for (int j = 5 - i; j < 16 + i; j++) {
            if ((j + i - 1) % 4 == 0) {
                field[i][j] = new Road(i, j, false, false);
                field[10 - i][j] = new Road(10 - i, j, false, false);
            } else if (j % 2 == 0) {
                field[i][j] = new Hexagon(i, j);
                field[10 - i][j] = new Hexagon(10 - i, j);
            } else {
                field[i][j] = nullptr;
                field[10 - i][j] = nullptr;
            }
        }
    }

    for (int i = 0; i < 5; i += 2) {
        bool direction = false;
        for (int j = 4 - i; j < 17 + i; j++) {
            if (j % 2 == 0) {
                field[i][j] = new Vertex(i, j, direction);
                field[10 - i][j] = new Vertex(10 - i, j, !direction);
            } else {
                field[i][j] = new Road(i, j, true, direction);
                field[10 - i][j] = new Road(10 - i, j, true, !direction);
                direction = !direction;
            }
        }
    }

    for (int j = 0; j < 21; j++) {
        if (j % 4 == 0) {
            field[5][j] = new Road(5, j, false, false);
        } else if (j % 2 == 0) {
            field[5][j] = new Hexagon(5, j);
        } else {
            field[5][j] = nullptr;
        }
    }

    //debug
    for (int i = 0; i < 11; i ++) {
        for (int j = 0; j < 21; j ++) {
            if (field[i][j] != nullptr) {
                std::cout << "X";
            } else {
                std::cout << '.';
            }
        }
        std::cout << '\n';
    }
    std::cout << '\n';
    std::cout << "Vertexes Graph:\n";
    for (int i = 0; i < 11; i ++) {
        for (int j = 0; j < 21; j ++) {
            if (field[i][j] != nullptr) {
                std::cout << '(' << i << ", " << j << ") :";
                for (auto v : field[i][j]->vertexes) {
                    std::cout << '(' << v.first << ", " << v.second << "), ";
                }
            } else {
                //std::cout << '.';
                continue;
            }
            std::cout << '\n';
        }
    }
    std::cout << '\n';
    std::cout << "Roads Graph:\n";
    for (int i = 0; i < 11; i ++) {
        for (int j = 0; j < 21; j ++) {
            if (field[i][j] != nullptr) {
                std::cout << '(' << i << ", " << j << ") :";
                for (auto v : field[i][j]->roads) {
                    std::cout << '(' << v.first << ", " << v.second << "), ";
                }
            } else {
                //std::cout << '.';
                continue;
            }
            std::cout << '\n';
        }
    }
    //debug end
}

Cell* Catan::getFieldCell(int x, int y) const {
    return field[x][y];
}
