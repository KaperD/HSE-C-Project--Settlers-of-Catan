#ifndef _BOARDS_H_
#define _BOARDS_H_

#include <vector>
#include <unordered_map>
#include <memory>
#include <random.h>

namespace Board {

constexpr int TERRITORIESNUM = 5;
constexpr int FIELDHEIGHT = 11;
constexpr int FIELDWIDTH = 21;
constexpr int HEXESNUM = 19;
constexpr int HEXVERTNUM = 6;

enum class PlayerNum {
    NONE,
    GAMER1,
    GAMER2,
    GAMER3,
    GAMER4
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
    explicit Cell(BuildingType type);

    PlayerNum getPlayer() const;
    void setPlayer(PlayerNum new_player);

    size_t getVertexNum() const;
    size_t getRoadsNum() const;
    std::pair<int, int> getRoad(int i) const;
    std::pair<int, int> getVertex(int i) const;
    BuildingType getType() const;
    void setBuildingType(BuildingType type);

    bool marked = false;

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
    Hexagon(int x, int y, int resource, int number);

    void moveRobbers();
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
    void delDevCard(DevelopmentCard dev_card);

    int getVictoryPoints() const;
    int getKnightsNum() const;
    int getResourceNum(Resource re) const;
    auto& getResources() const;
    auto& getDevCards() const;

    int checkResourceNum(Resource re);

private:
    PlayerNum id;
    int victory_points = 0;
    int knights = 0;
    mutable std::unordered_map<Resource, int> cards;
    mutable std::unordered_map<DevelopmentCard, int> dev_cards;
};

class Catan {
public:
    Catan(utility::Random& ran, int gamersNum_);

    void settle(BuildingType s, int x, int y);
    void giveResources(int cubes_num);
    Resource setRobbers(int hex_num);

    bool trade(Resource re_for_trade, Resource need_re);

    DevelopmentCard buildDevCard();
    void playDevCard(DevelopmentCard card, int extraData);

    bool canBuild(BuildingType mod, int x, int y) const;
    bool checkCards(BuildingType building);

    int getPlayerResNum(PlayerNum playerID, Resource re) const;
    const std::unordered_map<Resource, int>& getPlayerResources(PlayerNum playerID) const;
    const std::unordered_map<DevelopmentCard, int>& getPlayerDevCards(PlayerNum playerID) const;
    std::vector<int> getVictoryPoints() const;
    const std::unique_ptr<Cell>& getFieldCell(int x, int y) const;
    const std::unique_ptr<Hexagon>& getHex(int indx) const;
    int getRobbersIndx() const;

    void changeCurPlayer(PlayerNum new_player);

    int getRoadsRecord() const;
    PlayerNum getRoadsRecordHolder() const;

    void gotoNextGamePhase();
    bool isFinished();

private:
    std::vector<std::vector<std::unique_ptr<Cell>>> field;
    std::vector<std::unique_ptr<Hexagon>> hexes;
    mutable std::unordered_map<PlayerNum, std::unique_ptr<Player>> players;
    int robbers_hex = 17;
    PlayerNum cur_player = PlayerNum::NONE;
    int gamersNum = 0;
    bool is_beginning = true;

    PlayerNum last_roads_record_holder = PlayerNum::NONE;
    int roads_record = 4;
    PlayerNum last_knights_record_holder = PlayerNum::NONE;
    int knights_record = 2;

    std::vector<PlayerNum> playersIDs = {
            PlayerNum::NONE, PlayerNum::GAMER1,
            PlayerNum::GAMER2, PlayerNum::GAMER3,
            PlayerNum::GAMER4
    };

    void setKnightRecord(int new_record);

    int findRoadsRecord(const std::unique_ptr<Cell>& v);
    void setRoadsRecord(int new_record);
    void updateRoadsRecord();
    const std::unique_ptr<Cell>& getStart(const std::unique_ptr<Cell>& v, int x, int y, int dir);
    void clearMarks();

    utility::Random& random;
};

} // namespace Board

#endif
