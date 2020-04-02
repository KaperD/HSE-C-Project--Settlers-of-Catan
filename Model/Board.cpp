#include <cstdlib>
#include <ctime>
#include <memory>

#include "Board.h"

namespace Board {

#define cell(x, y) field[x][y]
#define getPlayerCardNum(card) players[cur_player]->checkResourceNum(card)

static bool check(int x, int y) {
    return !(x < 0 || y < 0 || x >= FIELDHEIGHT || y >= FIELDWIDTH  ||
          ((x < 2 || x > 8) && (y < 4 || y > 16)) ||
          ((x < 4 || x > 6) && (y < 2 || y > 18)));
}

//===============Player===================

Player::Player(PlayerNum id) : id(id), victory_points(0) {
    cards[Resource::WOOL] = 0;
    cards[Resource::ORE] = 0;
    cards[Resource::CLAY] = 0;
    cards[Resource::TREE] = 0;
    cards[Resource::WHEAT] = 0;

    dev_cards[DevelopmentCard::KNIGHT] = 0;
    dev_cards[DevelopmentCard::ROAD_BUILDING] = 0;
    dev_cards[DevelopmentCard::VICTORY_POINT] = 0;
    dev_cards[DevelopmentCard::MONOPOLY] = 0;
    dev_cards[DevelopmentCard::INVENTION] = 0;
}

void Player::giveResource(Resource re, int num) {
    cards[re] += num;
}

void Player::getResource(Resource re, int num) {
    cards[re] -= num;
}

int Player::checkResourceNum(Resource re) {
    return cards[re];
}

int Player::getVictoryPoints() const {
    return victory_points;
}

void Player::giveVictoryPoints(int vp) {
    victory_points += vp;
}

void Player::decrVictoryPoints(int vp) {
    victory_points -= vp;
}

int Player::getRoadsNum() const {
    return roads;
}

int Player::getKnightsNum() const {
    return knights;
}

void Player::incrArmy() {
    knights++;
}

void Player::addRoad() {
    roads++;
}

void Player::giveDevCard(DevelopmentCard dev_card) {
    cards[Resource::WHEAT]--;
    cards[Resource::WOOL]--;
    cards[Resource::ORE]--;
    dev_cards[dev_card]++;
}

void Player::delDevCard(DevelopmentCard dev_card) {
    dev_cards[dev_card]--;
}

//==============Cell==================

Cell::Cell(BuildingType type) : type(type) {}

PlayerNum Cell::getPlayer() const {
    return player;
}

BuildingType Cell::getType() const {
    return type;
}

void Cell::setBuildingType(BuildingType t) {
    type = t;
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

//================Vertex==================

Vertex::Vertex(int x, int y, bool direction) : Cell(BuildingType::VILLAGE) {
    if (direction) {
        if (check(x - 2, y)) {
            vertexes.emplace_back(x - 2, y);
        }
        if (check(x - 1, y)) {
            roads.emplace_back(x - 1, y);
        }
    } else {
        if (check(x + 2, y)) {
            vertexes.emplace_back(x + 2, y);
        }
        if (check(x + 1, y)) {
            roads.emplace_back(x + 1, y);
        }
    }
    if (check(x, y + 2)) {
        vertexes.emplace_back(x, y + 2);
    }
    if (check(x, y - 2)) {
        vertexes.emplace_back(x, y - 2);
    }
    if (check(x, y + 1)) {
        roads.emplace_back(x, y + 1);
    }
    if (check(x, y - 1)) {
        roads.emplace_back(x, y - 1);
    }
}

//===============Road=================

Road::Road(int x, int y, bool is_horizontal, bool is_even) : Cell(BuildingType::ROAD) {
    //тут много кода повторяется, потом поправлю
    if (is_horizontal) {
        vertexes.emplace_back(x, y - 1);
        vertexes.emplace_back(x, y + 1);
        if (check(x, y + 2)) {
            roads.emplace_back(x, y + 2);
        }
        if (check(x, y - 2)) {
            roads.emplace_back(x, y - 2);
        }
        if (is_even) {
            if (check(x + 1, y + 1)) {
                roads.emplace_back(x + 1, y + 1);
            }
            if (check(x - 1, y - 1)) {
                roads.emplace_back(x - 1, y - 1);
            }
        } else {
            if (check(x - 1, y + 1)) {
                roads.emplace_back(x - 1, y + 1);
            }
            if (check(x + 1, y - 1)) {
                roads.emplace_back(x + 1, y - 1);
            }
        }
    } else {
        vertexes.emplace_back(x + 1, y);
        vertexes.emplace_back(x - 1, y);
        if (check(x - 1, y + 1)) {
            roads.emplace_back(x - 1, y + 1);
        }
        if (check(x + 1, y - 1)) {
            roads.emplace_back(x + 1, y - 1);
        }
        if (check(x + 1, y + 1)) {
            roads.emplace_back(x + 1, y + 1);
        }
        if (check(x - 1, y - 1)) {
            roads.emplace_back(x - 1, y - 1);
        }
    }
}

//================Hexagon==================

Hexagon::Hexagon(int x, int y) : Cell(BuildingType::NONE) {
    vertexes.emplace_back(x + 1, y + 2);
    vertexes.emplace_back(x + 1, y);
    vertexes.emplace_back(x + 1, y - 2);
    vertexes.emplace_back(x - 1, y + 2);
    vertexes.emplace_back(x - 1, y);
    vertexes.emplace_back(x - 1, y - 2);

    int random_resource = rand() % TERRITORIESNUM + 1;
    re = static_cast<Resource>(random_resource);
    robbers = false;
    num = rand() % 11 + 2;
    if (num == 7) num++;
}

void Hexagon::setRobbers() {
    robbers = !robbers;
}

Resource Hexagon::getResource() const {
    return re;
}

int Hexagon::getNum() const {
    return num;
}

bool Hexagon::robbersIsHere() const {
    return robbers;
}

//=================Catan====================

Catan::Catan() : field(FIELDHEIGHT), players(4) {
    srand(time (nullptr));

    for (int i = 0; i < FIELDHEIGHT; i++) {
        for (int k = 0; k < FIELDWIDTH; k++) {
            field[i].push_back(nullptr);
        }
    }

    players[PlayerNum::GAMER1] = std::make_unique<Player>(PlayerNum::GAMER1);
    players[PlayerNum::GAMER2] = std::make_unique<Player>(PlayerNum::GAMER2);
    players[PlayerNum::GAMER3] = std::make_unique<Player>(PlayerNum::GAMER3);
    cur_player = PlayerNum::GAMER1;

    for (int i = 1; i < 4; i += 2) {
        for (int j = 5 - i; j < 16 + i; j++) {
            if ((j + i - 1) % 4 == 0) {
                cell(i, j) = std::make_unique<Road>(i, j, false, false);
                cell(10 - i, j) = std::make_unique<Road>(10 - i, j, false, false);
            } else if (j % 2 == 0) {
                Hexagon* hex_ptr1 = new Hexagon(i, j);
                Hexagon* hex_ptr2 = new Hexagon(10 - i, j);
                hexes.push_back(hex_ptr1);
                cell(i, j) = std::unique_ptr<Hexagon>(hex_ptr1);
                hexes.push_back(hex_ptr2);
                cell(10 - i, j) = std::unique_ptr<Hexagon>(hex_ptr2);
            }
        }
    }

    for (int i = 0; i < 5; i += 2) {
        bool direction = false;
        for (int j = 4 - i; j < 17 + i; j++) {
            if (j % 2 == 0) {
                cell(i, j) = std::make_unique<Vertex>(i, j, direction);
                cell(10 - i, j) = std::make_unique<Vertex>(10 - i, j, !direction);
            } else {
                cell(i, j) = std::make_unique<Road>(i, j, true, direction);
                cell(10 - i, j) = std::make_unique<Road>(10 - i, j, true, !direction);
                direction = !direction;
            }
        }
    }

    for (int j = 0; j < FIELDWIDTH; j++) {
        if (j % 4 == 0) {
            cell(5, j) = std::make_unique<Road>(5, j, false, false);
        } else if (j % 2 == 0) {
            Hexagon* hex_ptr = new Hexagon(5, j);
            hexes.push_back(hex_ptr);
            cell(5, j) = std::unique_ptr<Hexagon>(hex_ptr);
        }
    }

    robbers_hex = rand() % (TERRITORIESNUM) + 1;
    hexes[robbers_hex]->setRobbers();

}

const std::unique_ptr<Cell>& Catan::getFieldCell(int x, int y) const {
    return cell(x, y);
}

Hexagon* Catan::getHex(int indx) const {
    if (indx > HEXESNUM || indx < 0) return nullptr;
    return hexes[indx];
}

bool Catan::canBuild(BuildingType mod, int x, int y) const {
    if (cell(x, y) == nullptr ||
        (cell(x, y)->getPlayer() != PlayerNum::NONE &&
        cell(x, y)->getPlayer() != cur_player) ||
        cell(x, y)->getType() != mod) {
        return false;
    }

    size_t lenV = cell(x, y)->getVertexNum();
    size_t lenR = cell(x, y)->getRoadsNum();

    if (mod == BuildingType::VILLAGE || mod == BuildingType::CITY) {
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
        if (cell(neighbourV.first, neighbourV.second)->getPlayer() == cur_player) {
            return true;
        }
    }

    for (size_t i = 0; i < lenR; i++) {
        int neighbourRx = cell(x, y)->getRoad(i).first;
        int neighbourRy = cell(x, y)->getRoad(i).second;
        if (cell(neighbourRx, neighbourRy)->getPlayer() == cur_player) {
            size_t neigh_lenV = cell(neighbourRx, neighbourRy)->getVertexNum();
            for (size_t j = 0; j < neigh_lenV; j++) {
                int vx = cell(neighbourRx, neighbourRy)->getVertex(j).first;
                int vy = cell(neighbourRx, neighbourRy)->getVertex(j).second;
                if (cell(vx, vy)->getPlayer() != PlayerNum::NONE &&
                    cell(vx, vy)->getPlayer() != cur_player) {
                    return false;
                }
            }
            return true;
        }
    }

    return false;
}

bool Catan::checkCards(BuildingType building) {
    if (building == BuildingType::VILLAGE) {
        return getPlayerCardNum(Resource::TREE) > 0 && getPlayerCardNum(Resource::CLAY) > 0 &&
               getPlayerCardNum(Resource::WOOL) > 0 && getPlayerCardNum(Resource::WHEAT) > 0;
    }
    if (building == BuildingType::CITY) {
        return getPlayerCardNum(Resource::ORE) >= 3 && getPlayerCardNum(Resource::WHEAT) >= 2;
    }
    return getPlayerCardNum(Resource::TREE) > 0 && getPlayerCardNum(Resource::CLAY) > 0;
}

void Catan::settle(BuildingType s, int x, int y) {
    if (s == BuildingType::VILLAGE) {
        players[cur_player]->getResource(Resource::TREE, 1);
        players[cur_player]->getResource(Resource::CLAY, 1);
        players[cur_player]->getResource(Resource::WOOL, 1);
        players[cur_player]->getResource(Resource::WHEAT, 1);
    } else if (s == BuildingType::CITY) {
        players[cur_player]->getResource(Resource::ORE, 3);
        players[cur_player]->getResource(Resource::WHEAT, 2);
    } else {
        players[cur_player]->getResource(Resource::TREE, 1);
        players[cur_player]->getResource(Resource::CLAY, 1);
        players[cur_player]->addRoad();
    }
    cell(x, y)->setPlayer(cur_player);
    cell(x, y)->setBuildingType(s);
}

void Catan::setRobbers(int hex_num) {
    hexes[robbers_hex]->setRobbers();
    hexes[hex_num]->setRobbers();
    robbers_hex = hex_num;
}

void Catan::changeCurPlayer(PlayerNum new_player) {
    cur_player = new_player;
}

void Catan::giveResources(int cubes_num) {
    for (auto h : hexes) {
        if (cubes_num != h->getNum() || h->robbersIsHere()) continue;
        for (int i = 0; i < 7; i++) {
            int vx = h->getVertex(i).first;
            int vy = h->getVertex(i).second;
            if (cell(vx, vy)->getPlayer() != PlayerNum::NONE) {
                int re_num = 1;
                if (cell(vx, vy)->getType() == BuildingType::CITY) {
                    re_num = 2;
                }
                players[cell(vx, vy)->getPlayer()]->giveResource(h->getResource(), re_num);
            }
        }
    }
}

bool Catan::trade(Resource re_for_trade, Resource need_re) {
    if (getPlayerCardNum(re_for_trade) < 4) {
        return false;
    }
    players[cur_player]->getResource(re_for_trade, 4);
    players[cur_player]->giveResource(need_re, 1);
    return true;
}

int Catan::getRoadsRecord() const {
    return roads_record;
}

int Catan::getKnightRecord() const {
    return knights_record;
}

void Catan::setRoadsRecord(int new_record) {
    players[last_roads_record_holder]->decrVictoryPoints(2);
    players[cur_player]->giveVictoryPoints(2);
    last_roads_record_holder = cur_player;
    roads_record = new_record;
}

void Catan::setKnightRecord(int new_record) {
    players[last_knights_record_holder]->decrVictoryPoints(2);
    players[cur_player]->giveVictoryPoints(2);
    last_knights_record_holder = cur_player;
    knights_record = new_record;
}

bool Catan::isFinished() {
    return players[PlayerNum::GAMER1]->getVictoryPoints() == 10 ||
           players[PlayerNum::GAMER2]->getVictoryPoints() == 10 ||
           players[PlayerNum::GAMER3]->getVictoryPoints() == 10;
}

}
