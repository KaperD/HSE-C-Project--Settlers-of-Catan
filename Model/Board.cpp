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
    cards[Resource::WOOL] = 2;
    cards[Resource::ORE] = 0;
    cards[Resource::CLAY] = 4;
    cards[Resource::TREE] = 4;
    cards[Resource::WHEAT] = 2;

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

int Player::getKnightsNum() const {
    return knights;
}

void Player::incrArmy() {
    knights++;
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

int Player::getResourceNum(Resource re) const {
    return cards[re];
}

auto& Player::getResources() const {
    return cards;
}

auto& Player::getDevCards() const {
    return dev_cards;
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

Hexagon::Hexagon(int x, int y, int resource, int number) : Cell(BuildingType::NONE) {
    vertexes.emplace_back(x + 1, y + 2);
    vertexes.emplace_back(x + 1, y);
    vertexes.emplace_back(x + 1, y - 2);
    vertexes.emplace_back(x - 1, y + 2);
    vertexes.emplace_back(x - 1, y);
    vertexes.emplace_back(x - 1, y - 2);

    re = static_cast<Resource>(resource);
    num = number;
    robbers = re == Resource::NONE;
}

void Hexagon::moveRobbers() {
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

Catan::Catan(utility::Random& ran, int gamersNum_) : field(FIELDHEIGHT), players(4),
                                                     gamersNum(gamersNum_), random(ran) {

    for (int i = 0; i < FIELDHEIGHT; i++) {
        for (int k = 0; k < FIELDWIDTH; k++) {
            field[i].push_back(nullptr);
        }
    }

    for (int i = 0; i <= gamersNum; i++) {
        auto playerID = playersIDs[i];
        players[playerID] = std::make_unique<Player>(playerID);
    }
    cur_player = PlayerNum::GAMER1;

    auto randomResouresAndNumbers = random.generateResourcesAndNumbers();
    auto curResourseAndNumber = std::begin(randomResouresAndNumbers);

    for (int i = 1; i < 4; i += 2) {
        for (int j = 5 - i; j < 16 + i; j++) {
            if ((j + i - 1) % 4 == 0) {
                cell(i, j) = std::make_unique<Road>(i, j, false, false);
                cell(10 - i, j) = std::make_unique<Road>(10 - i, j, false, false);
            } else if (j % 2 == 0) {
                hexes.push_back(std::make_unique<Hexagon>(i, j, curResourseAndNumber->resource,
                                                                curResourseAndNumber->number));
                ++curResourseAndNumber;
                hexes.push_back(std::make_unique<Hexagon>(10 - i, j, curResourseAndNumber->resource,
                                                                     curResourseAndNumber->number));
                ++curResourseAndNumber;
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
            hexes.push_back(std::make_unique<Hexagon>(5, j, curResourseAndNumber->resource,
                                                            curResourseAndNumber->number));
            ++curResourseAndNumber;
        }
    }

    for (int i = 0; i < HEXESNUM; i++) {
        if (hexes[i]->robbersIsHere()) {
            robbers_hex = i;
            break;
        }
    }

}

const std::unique_ptr<Cell>& Catan::getFieldCell(int x, int y) const {
    return cell(x, y);
}

const std::unique_ptr<Hexagon>& Catan::getHex(int indx) const {
    return hexes[indx];
}

void Catan::gotoNextGamePhase() {
    is_beginning = false;
}

bool Catan::canBuild(BuildingType mod, int x, int y) const {
    if (cell(x, y) == nullptr) return false;

    if (mod == BuildingType::CITY)
        return cell(x, y)->getType() == BuildingType::VILLAGE && cell(x, y)->getPlayer() == cur_player;

    if (cell(x, y)->getType() != mod) return false;

    size_t lenV = cell(x, y)->getVertexNum();
    size_t lenR = cell(x, y)->getRoadsNum();


    if (cell(x, y)->getPlayer() != PlayerNum::NONE) return false;

    if (mod == BuildingType::VILLAGE) {
        for (size_t i = 0; i < lenV; i++) {
            std::pair<int, int> neighbour = cell(x, y)->getVertex(i);
            if (cell(neighbour.first, neighbour.second)->getPlayer() != PlayerNum::NONE) {
                return false;
            }
        }
        if (is_beginning) return true;
        for (size_t i = 0; i < lenR; i++) {
            std::pair<int, int> neighbour = cell(x, y)->getRoad(i);
            if (cell(neighbour.first, neighbour.second)->getPlayer() == cur_player) {
                return true;
            }
        }
        return false;
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
    if (building == BuildingType::DevCard) {
        return getPlayerCardNum(Resource::ORE) > 0 &&
               getPlayerCardNum(Resource::WOOL) > 0 &&
               getPlayerCardNum(Resource:: WHEAT) > 0;
    }
    return getPlayerCardNum(Resource::TREE) > 0 && getPlayerCardNum(Resource::CLAY) > 0;
}

void Catan::settle(BuildingType s, int x, int y) {
    if (s == BuildingType::VILLAGE) {
        players[cur_player]->getResource(Resource::TREE, 1);
        players[cur_player]->getResource(Resource::CLAY, 1);
        players[cur_player]->getResource(Resource::WOOL, 1);
        players[cur_player]->getResource(Resource::WHEAT, 1);
        cell(x, y)->setPlayer(cur_player);
        players[cur_player]->giveVictoryPoints(1);
    } else if (s == BuildingType::CITY) {
        players[cur_player]->getResource(Resource::ORE, 3);
        players[cur_player]->getResource(Resource::WHEAT, 2);
        cell(x, y)->setBuildingType(BuildingType::CITY);
        players[cur_player]->giveVictoryPoints(2);
    } else {
        players[cur_player]->getResource(Resource::TREE, 1);
        players[cur_player]->getResource(Resource::CLAY, 1);
        cell(x, y)->setPlayer(cur_player);

        updateRoadsRecord();
    }
}

int Catan::findRoadsRecord(const std::unique_ptr<Cell> &v) {
    if (v->getPlayer() != cur_player && v->getPlayer() != PlayerNum::NONE) return -1;
    if (v->marked) return 0;


    v->marked = true;
    std::vector<int> roadsNum(3);
    int numR = v->getRoadsNum();

    for (int i = 0; i < numR; i++) {
        int rx = v->getRoad(i).first;
        int ry = v->getRoad(i).second;

        if (cell(rx, ry)->marked || cell(rx, ry)->getPlayer() != cur_player) {
            continue;
        }
        cell(rx, ry)->marked = true;

        for (int k = 0; k < 2; k++) {
            int vx = cell(rx, ry)->getVertex(k).first;
            int vy = cell(rx, ry)->getVertex(k).second;
            if (cell(vx, vy) == v || cell(vx, vy)->marked) continue;
            roadsNum[i] += findRoadsRecord(cell(vx, vy)) + 1;
        }
    }

    int sum = 0;
    std::vector<int> sumRoads(3);
    int maxRoadsNum = 0;
    sumRoads[0] = roadsNum[0] + roadsNum[1];
    sumRoads[1] = roadsNum[0] + roadsNum[2];
    sumRoads[2] = roadsNum[2] + roadsNum[1];
    sum = std::max(sumRoads[0], std::max(sumRoads[1], sumRoads[2]));
    if (sum > roads_record) {
        setRoadsRecord(sum);
    }

    for (auto roads : roadsNum) {
        if (roads > maxRoadsNum) maxRoadsNum = roads;
    }

    return maxRoadsNum;
}

const std::unique_ptr<Cell>& Catan::getStart(const std::unique_ptr<Cell> &v, int x, int y, int dir) {
    if (y + dir*2 < 0 || cell(x, y + dir) == nullptr || cell(x, y + dir)->marked ||
        cell(x, y + dir)->getPlayer() != cur_player || cell(x, y + dir * 2) == nullptr ||
        (cell(x, y + dir * 2)->getPlayer() != cur_player &&
        cell(x, y + dir * 2)->getPlayer() != PlayerNum::NONE)) {
        if (dir == 1) return v;
        int numR = v->getRoadsNum();
        int cnt = 0;
        for (int i = 0; i < numR; i++) {
            int rx = v->getRoad(i).first;
            int ry = v->getRoad(i).second;
            if (cell(rx, ry)->getPlayer() == cur_player) cnt++;
        }
        if (cnt >= 2) return getStart(cell(x, y + 2), x, y + 2, 1);
        return v;
    }

    return getStart(cell(x, y + dir * 2), x, y + dir * 2, dir);
}

void Catan::clearMarks() {
    for (int x = 0; x < FIELDHEIGHT; x++) {
        for (int y = 0; y < FIELDWIDTH; y++) {
            if (cell(x, y) != nullptr) {
                cell(x, y)->marked = false;
            }
        }
    }
}

void Catan::updateRoadsRecord() {
    int maybeRecord;

    for (int vx = 0; vx < FIELDHEIGHT; vx += 2) {
        for (int vy = 0; vy < FIELDWIDTH; vy += 2) {
            if (cell(vx, vy) == nullptr || cell(vx, vy)->marked ||
                cell(vx, vy)->getPlayer() != cur_player) continue;
            maybeRecord = findRoadsRecord(getStart(cell(vx, vy), vx, vy, -1));
            if (maybeRecord > roads_record) {
                setRoadsRecord(maybeRecord);
            }
        }
    }

    clearMarks();
}

Resource Catan::setRobbers(int hex_num) {
    hexes[robbers_hex]->moveRobbers();
    hexes[hex_num]->moveRobbers();
    robbers_hex = hex_num;

    int x, y;
    for (int i = 0; i < HEXVERTNUM; i++) {
        x = hexes[hex_num]->getVertex(i).first;
        y = hexes[hex_num]->getVertex(i).second;
        PlayerNum player = cell(x, y)->getPlayer();
        if (player != PlayerNum::NONE && player != cur_player) {
            for (int intRE = 1; intRE <= TERRITORIESNUM; intRE++) {
                auto re = static_cast<Resource>(intRE);
                if (players[player]->checkResourceNum(re) > 0) {
                    players[player]->getResource(re, 1);
                    players[cur_player]->giveResource(re, 1);
                    return re;
                }
            }
        }
    }
    return Resource::NONE;
}

int Catan::getRobbersIndx() const {
    return robbers_hex;
}

void Catan::changeCurPlayer(PlayerNum new_player) {
    cur_player = new_player;
}

void Catan::giveResources(int cubes_num) {
    for (int i = 0; i < HEXESNUM; i++) {
        if (cubes_num != hexes[i]->getNum() || hexes[i]->robbersIsHere()) continue;
        for (int j = 0; j < HEXVERTNUM; j++) {
            int vx = hexes[i]->getVertex(j).first;
            int vy = hexes[i]->getVertex(j).second;
            if (cell(vx, vy)->getPlayer() != PlayerNum::NONE) {
                int re_num = 1;
                if (cell(vx, vy)->getType() == BuildingType::CITY) {
                    re_num = 2;
                }
                players[cell(vx, vy)->getPlayer()]->giveResource(hexes[i]->getResource(), re_num);
            }
        }
    }
}

int Catan::getPlayerResNum(PlayerNum playerID, Resource re) const {
    return players[playerID]->getResourceNum(re);
}

const std::unordered_map<Resource, int>& Catan::getPlayerResources(PlayerNum playerID) const {
    return players[playerID]->getResources();
}

bool Catan::trade(Resource re_for_trade, Resource need_re) {
    if (getPlayerCardNum(re_for_trade) < 3) {
        return false;
    }
    players[cur_player]->getResource(re_for_trade, 3);
    players[cur_player]->giveResource(need_re, 1);
    return true;
}

const std::unordered_map<DevelopmentCard, int>& Catan::getPlayerDevCards(PlayerNum playerID) const {
    return players[playerID]->getDevCards();
}

std::vector<int> Catan::getVictoryPoints() const {
    std::vector<int> VP;
    for (int i = 1; i <= gamersNum; i++) {
        auto playerID = playersIDs[i];
        VP.push_back(players[playerID]->getVictoryPoints());
    }
    return VP;
}

DevelopmentCard Catan::buildDevCard() {
    if (getPlayerCardNum(Resource::ORE) < 1 ||
        getPlayerCardNum(Resource::WHEAT) < 1 ||
        getPlayerCardNum(Resource::WOOL) < 1) {
        return DevelopmentCard::NONE;
    }
    auto card = static_cast<DevelopmentCard>(random.getRandomNumberFromTo(1, 5));
    players[cur_player]->giveDevCard(card);
    return card;
}

void Catan::playDevCard(DevelopmentCard card, int extraData) {
    if (card == DevelopmentCard::KNIGHT) {
        setRobbers(extraData);
        players[cur_player]->incrArmy();
        if (players[cur_player]->getKnightsNum() > knights_record) {
            setKnightRecord(knights_record = players[cur_player]->getKnightsNum());
        }
    }
    if (card == DevelopmentCard::VICTORY_POINT) {
        players[cur_player]->giveVictoryPoints(1);
    }
    if (card == DevelopmentCard::INVENTION) {
        auto re1 = static_cast<Resource>(random.getRandomNumberFromTo(1, TERRITORIESNUM));
        auto re2 = static_cast<Resource>(random.getRandomNumberFromTo(1, TERRITORIESNUM));
        players[cur_player]->giveResource(re1, 1);
        players[cur_player]->giveResource(re2, 1);
    }
    if (card == DevelopmentCard::ROAD_BUILDING) {
        players[cur_player]->giveResource(Resource::TREE, 2);
        players[cur_player]->giveResource(Resource::CLAY, 2);
    }
    if (card == DevelopmentCard::MONOPOLY) {
        auto re = static_cast<Resource>(extraData);
        for (int intPlayer = 1; intPlayer < 3; intPlayer++) {
            auto player = static_cast<PlayerNum>(intPlayer);
            int reNum = players[player]->checkResourceNum(re);
            players[player]->getResource(re, reNum);
            players[cur_player]->giveResource(re, reNum);
        }
    }
    players[cur_player]->delDevCard(card);
}

int Catan::getRoadsRecord() const {
    return roads_record;
}

PlayerNum Catan::getRoadsRecordHolder() const {
    return last_roads_record_holder;
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
    for(int i = 1; i <= gamersNum; i++) {
        if (players[playersIDs[i]]->getVictoryPoints() >= 10) return true;
    }
    return false;
}

}
