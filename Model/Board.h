#ifndef _BOARDS_H_
#define _BOARDS_H_

#include <vector>
#include <unordered_map>
#include <memory>

namespace Board {

const int TERRITORIESNUM = 5;
const int VERTEXNUM = 54;
const int FIELDHEIGHT = 11;
const int FIELDWIDTH = 21;
const int HEXESNUM = 19;

enum class PlayerNum {
    NONE,
    GAMER1,
    GAMER2,
    GAMER3
};

enum class Resource {
    NONE,
    WOOL, //овцы
    ORE,
    CLAY, //глина
    TREE,
    WHEAT //пшеница
};

enum class DevelopmentCard {
    NONE,
    KNIGHT,
    VICTORY_POINT,
    ROAD_BUILDING,
    MONOPOLY,
    INVENTION
};

enum class BuildingType {
    NONE,
    VILLAGE,
    CITY,
    ROAD,
    DevCard
};

class Cell {
public:
    Cell(BuildingType type);

    PlayerNum getPlayer() const;
    void setPlayer(PlayerNum new_player);

    size_t getVertexNum() const;
    size_t getRoadsNum() const;
    std::pair<int, int> getRoad(int i) const;
    std::pair<int, int> getVertex(int i) const;
    BuildingType getType() const;
    void setBuildingType(BuildingType type);

protected:

    BuildingType type = BuildingType::NONE;
    PlayerNum player = PlayerNum::NONE;
    std::vector<std::pair<int, int>> vertexes;
    std::vector<std::pair<int, int>> roads;
};

class Vertex : public Cell {
public:
    Vertex(int x, int y, bool direction);

};

class Road : public Cell {
public:
    Road(int x, int y, bool is_horizontal, bool is_even);

};

class Hexagon : public Cell {
public:
    Hexagon(int x, int y);

    void setRobbers();
    Resource getResource() const;
    int getNum() const;
    bool robbersIsHere() const;

private:
    Resource re;
    int num;
    bool robbers;
};

class Player {
public:
    Player(PlayerNum id);

    void giveResource(Resource re, int num);
    void getResource(Resource re, int num);
    void giveVictoryPoints(int vp);
    void decrVictoryPoints(int vp);
    void giveDevCard(DevelopmentCard dev_card);
    void incrArmy();
    void addRoad();
    void delDevCard(DevelopmentCard dev_card);

    int getVictoryPoints() const;
    int getKnightsNum() const;
    int getRoadsNum() const;

    int checkResourceNum(Resource re);

private:
    PlayerNum id;
    int victory_points;
    int roads = 0;
    int knights = 0;
    std::unordered_map<Resource, int> cards;
    std::unordered_map<DevelopmentCard, int> dev_cards;
};

class Catan {
public:
    Catan();

    void settle(BuildingType s, int x, int y);
    void giveResources(int cubes_num);
    //возвращает true, если торговля прошла успешно, false, если не хватило ресурсов на обмен
    bool trade(Resource re_for_trade, Resource need_re);

    //возвращает true или false аналогично торговле
    bool buildDevCard();
    void playDevCard(DevelopmentCard card, int extraData);

    bool canBuild(BuildingType mod, int x, int y) const;
    bool checkCards(BuildingType building);

    const std::unique_ptr<Cell>& getFieldCell(int x, int y) const;
    void changeCurPlayer(PlayerNum new_player);
    void setRobbers(int hex_num);
    Hexagon* getHex(int indx) const;

    int getRoadsRecord() const;
    int getKnightRecord() const;
    void setRoadsRecord(int new_record);
    void setKnightRecord(int new_record);

    bool isFinished();

private:
    std::vector<std::vector<std::unique_ptr<Cell>>> field;
    std::vector<Hexagon*> hexes;
    std::unordered_map<PlayerNum, std::unique_ptr<Player>> players;
    int robbers_hex;
    PlayerNum cur_player;

    PlayerNum last_roads_record_holder = PlayerNum::NONE;
    int roads_record = 4;
    PlayerNum last_knights_record_holder = PlayerNum::NONE;
    int knights_record = 2;
};

} // namespace Board

#endif
