#ifndef _GAMECONTROLLER_H_
#define _GAMECONTROLLER_H_

#include <vector>
#include <memory>

#include "game.grpc.pb.h"
#include "Board.h"
#include "Client.h"

class Handler {
public:
    virtual void processEvent(::game::Event& event) = 0;
    virtual void displayEvent(::game::Event& event) = 0;
    virtual void Event(::game::Event& event) = 0;
private:
    int currentPlayer_;
};

class CardHandler : public Handler {
public:
    void processEvent(::game::Event& event) override;
    void displayEvent(::game::Event& event) override;
    void Event(::game::Event& event) override;
private:
    int cardType_;
};

class DiceHandler : public Handler {
public:
    void processEvent(::game::Event& event) override;
    void displayEvent(::game::Event& event) override;
    void Event(::game::Event& event) override;
private:
    int number_;
};

class MarketHandler : public Handler {
public:
    void processEvent(::game::Event& event) override;
    void displayEvent(::game::Event& event) override;
    void Event(::game::Event& event) override;
private:
    int requiredResource_;
    int ownedResource_;
};

class BuildHandler : public Handler {
public:
    void processEvent(::game::Event& event) override;
    void displayEvent(::game::Event& event) override;
    void Event(::game::Event& event) override;
private:
    int x_;
    int y_;
};

class GameController final {
public: 
    GameController();

    GameController(const GameController&) = delete;
    GameController operator=(const GameController&) = delete;

    void RunGame();


private:
    std::vector<std::unique_ptr<Handler>> handlers_;
    Catan gameModel_;
    // ViewModel m_;
    GameClient gameClient_;
    int myTurn_;
    int currentTurn_;
};

#endif