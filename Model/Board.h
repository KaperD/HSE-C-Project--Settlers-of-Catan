#ifndef _BOARDS_H_
#define _BOARDS_H_

#include <vector>
#include <unordered_map>
#include <memory>
#include <random.h>

namespace Board {

constexpr int TERRITORIESNUM = 5;
constexpr int VERTEXNUM = 54;
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
    int getDevCardNum(DevelopmentCard dev_card) const;
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
    void setRobbers(int hex_num);

    //возвращает true, если торговля прошла успешно, false, если не хватило ресурсов на обмен
    bool trade(Resource re_for_trade, Resource need_re);
    //TODO: доработать логику торговли с другими игроками, сейчас пока сыро
    void tradeWith(PlayerNum customerID, Resource re_for_trade, int tradeReNum, Resource need_re, int needReNum);


    //возвращает true или false аналогично торговле
    bool buildDevCard();
    void playDevCard(DevelopmentCard card, int extraData);

    bool canBuild(BuildingType mod, int x, int y) const;
    bool checkCards(BuildingType building);

    int getPlayerResNum(PlayerNum playerID, Resource re) const;
    int getPlayerDevCardNum(PlayerNum playerID, DevelopmentCard devCard) const;
    const std::unordered_map<Resource, int>& getPlayerResources(PlayerNum playerID) const;
    const std::unordered_map<DevelopmentCard, int>& getPlayerDevCards(PlayerNum playerID) const;
    std::vector<int> getVictoryPoints() const;
    const std::unique_ptr<Cell>& getFieldCell(int x, int y) const;
    const std::unique_ptr<Hexagon>& getHex(int indx) const;
    PlayerNum getCurPlayer() const;
    int getRobbersIndx() const;

    void changeCurPlayer(PlayerNum new_player);
    void nextPlayer();

    int getRoadsRecord() const;
    PlayerNum getRoadsRecordHolder() const;
    int getKnightRecord() const;
    PlayerNum getKnightRecordHolder() const;
    void setRoadsRecord(int new_record);
    void setKnightRecord(int new_record);

    void gotoNextGamePhase();
    bool isBeginning() const;
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

    //TODO: static_cast<enum class> VS vector<enum class>
    std::vector<PlayerNum> playersIDs = {
            PlayerNum::NONE, PlayerNum::GAMER1,
            PlayerNum::GAMER2, PlayerNum::GAMER3,
            PlayerNum::GAMER4
    };

    int findRoadsRecord(const std::unique_ptr<Cell>& v);
    void updateRoadsRecord();
    const std::unique_ptr<Cell>& getStart(const std::unique_ptr<Cell>& v, int x, int y);
    void clearMarks();

    utility::Random& random;
};

} // namespace Board

#endif
