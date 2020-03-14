#include <cstdlib>
#include <ctime>
#include <iostream> //for debug

#include "Board.h"

namespace Board {

#define getCellPlayer(v) board.getFieldCell(v.first, v.second).getPlayer()
#define cell(x, y) field[x][y]

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

size_t Cell::getVertexNum() const {
    return vertexes.size();
}

size_t Cell::getRoadsNum() const {
    return roads.size();
}

std::pair<int, int> Cell::getRoad(int i) const {
    return roads[i];
}

std::pair<int, int> Cell::getVertex(int i) const {
    return vertexes[i];
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

Catan::Catan() : field(11), players(3) {
    srand(time (NULL));
    for (int i = 0; i < 11; ++i) {
        for (int k = 0; k < 21; ++k) {
            field[i].push_back(nullptr);
        }
    }

    players[0] = std::unique_ptr<Player>(new Player(PlayerNum::GAMER1));
    players[1] = std::unique_ptr<Player>(new Player(PlayerNum::GAMER2));
    players[2] = std::unique_ptr<Player>(new Player(PlayerNum::GAMER3));
    cur_player = PlayerNum::GAMER1;

    for (int i = 1; i < 4; i += 2) {
        for (int j = 5 - i; j < 16 + i; j++) {
            if ((j + i - 1) % 4 == 0) {
                field[i][j] = std::unique_ptr<Road>(new Road(i, j, false, false));
                field[10 - i][j] = std::unique_ptr<Road>(new Road(10 - i, j, false, false));
            } else if (j % 2 == 0) {
                field[i][j] = std::unique_ptr<Hexagon>(new Hexagon(i, j));
                field[10 - i][j] = std::unique_ptr<Hexagon>(new Hexagon(10 - i, j));
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
                field[i][j] = std::unique_ptr<Vertex>(new Vertex(i, j, direction));
                field[10 - i][j] = std::unique_ptr<Vertex>(new Vertex(10 - i, j, !direction));
            } else {
                field[i][j] = std::unique_ptr<Road>(new Road(i, j, true, direction));
                field[10 - i][j] = std::unique_ptr<Road>(new Road(10 - i, j, true, !direction));
                direction = !direction;
            }
        }
    }

    for (int j = 0; j < 21; j++) {
        if (j % 4 == 0) {
            field[5][j] = std::unique_ptr<Road>(new Road(5, j, false, false));
        } else if (j % 2 == 0) {
            field[5][j] = std::unique_ptr<Hexagon>(new Hexagon(5, j));
        } else {
            field[5][j] = nullptr;
        }
    }

    //говнокод + нужен рандом
    Hexagon* desert = static_cast<Hexagon*>(field[5][10].get());
    desert->setRobbers();

}

const std::unique_ptr<Cell>& Catan::getFieldCell(int x, int y) const {
    return field[x][y];
}

bool Catan::canBuild(BuildingType mod, PlayerNum player, int x, int y) const {
    //обсудить с ребятами, не говнокод ли это и вообще ок столько геттеров ради такого делать
    if (cell(x, y)->getPlayer() != PlayerNum::NONE &&
        cell(x, y)->getPlayer() != player) {
        return false;
    }

    size_t lenV = cell(x, y)->getVertexNum();
    size_t lenR = cell(x, y)->getRoadsNum();

    if (mod == BuildingType::SETTLEMENT) {
        for (size_t i = 0; i < lenV; i++) {
            std::pair<int, int> neighbour = cell(x, y)->getVertex(i);
            if (cell(neighbour.first, neighbour.second)->getPlayer() != PlayerNum::NONE) {
                return false;
            }
        }
        return true;
    }

    for (size_t i = 0; i < lenV; i++) {
        std::pair<int, int> neighbourV = cell(x, y)->getVertex(i);
        if (cell(neighbourV.first, neighbourV.second)->getPlayer() == player) {
            return true;
        }
    }

    for (size_t i = 0; i < lenR; i++) {
        std::pair<int, int> neighbourR = cell(x, y)->getRoad(i);
        if (cell(neighbourR.first, neighbourR.second)->getPlayer() == player) {
            size_t neigh_lenV = cell(neighbourR.first, neighbourR.second)->getVertexNum();
            for (size_t j = 0; j < neigh_lenV; j++) {
                std::pair<int, int> v = cell(neighbourR.first, neighbourR.second)->getVertex(j);
                if (cell(v.first, v.second)->getPlayer() != PlayerNum::NONE &&
                    cell(v.first, v.second)->getPlayer() != player) {
                    return false;
                }
            }
            return true;
        }
    }

    return false;
}

} // namespace Board
//обсудить также предстоящие статик касты или виртуальные мутки, опять же получается говнокод какой-то либо я себя накручиваю
