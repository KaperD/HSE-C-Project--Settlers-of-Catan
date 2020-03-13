#ifndef _GAMECONTROLLER_H_
#define _GAMECONTROLLER_H_

#include <vector>
#include <memory>

#include "game.grpc.pb.h"
#include "Board.h"
#include "Client.h"
#include "Client.h"
#include "GUI.h"

namespace Controller {
/**
 * @brief Базовый класс для обработчиков событий
 *
 */
class Handler {
public:
    Handler(Board::Catan& model, View& view, GameClient& client)
    : currentPlayer_(0)
    , gameModel_(model)
    , gameView_(view)
    , gameClient_(client) { }
    virtual void processEvent(::game::Event& event) = 0; ///< @brief Вызывает нужные команды у модели
    virtual void displayEvent(::game::Event& event) = 0; ///< @brief Вызывает нужные команды у GUI
    void sendEvent(::game::Event& event); ///< @brief Вызывает нужные команды у клиента
protected:
    int currentPlayer_;
    Board::Catan& gameModel_;
    View& gameView_;
    GameClient& gameClient_;
};

class CardHandler : public Handler {
public:
    CardHandler(Board::Catan& model, View& view, GameClient& client)
    : Handler(model, view, client)
    , cardType_(0) { }
    void processEvent(::game::Event& event) override;
    void displayEvent(::game::Event& event) override;
    using Handler::sendEvent;
private:
    int cardType_;
};

class DiceHandler : public Handler {
public:
    DiceHandler(Board::Catan& model, View& view, GameClient& client)
    : Handler(model, view, client)
    , number_(0) { }
    void processEvent(::game::Event& event) override;
    void displayEvent(::game::Event& event) override;
    using Handler::sendEvent;
private:
    int number_;
};

class MarketHandler : public Handler {
public:
    MarketHandler(Board::Catan& model, View& view, GameClient& client)
    : Handler(model, view, client)
    , requiredResource_(0)
    , ownedResource_(0) { }
    void processEvent(::game::Event& event) override;
    void displayEvent(::game::Event& event) override;
    using Handler::sendEvent;
private:
    int requiredResource_;
    int ownedResource_;
};

class BuildHandler : public Handler {
public:
    BuildHandler(Board::Catan& model, View& view, GameClient& client)
    : Handler(model, view, client)
    , buildingType_(0)
    , x_(0)
    , y_(0) { }
    void processEvent(::game::Event& event) override;
    void displayEvent(::game::Event& event) override;
    using Handler::sendEvent;
private:
    int buildingType_;
    int x_;
    int y_;
};

class EndTurnHandler : public Handler {
public:
    EndTurnHandler(Board::Catan& model, View& view, GameClient& client)
    : Handler(model, view, client) { }
    void processEvent(::game::Event& event) override;
    void displayEvent(::game::Event& event) override;
    using Handler::sendEvent;
};

class NextPhaseHandler : public Handler {
public:
    NextPhaseHandler(Board::Catan& model, View& view, GameClient& client)
    : Handler(model, view, client) { }
    void processEvent(::game::Event& event) override;
    void displayEvent(::game::Event& event) override;
    using Handler::sendEvent;
};

class GameController final {
public: 
    GameController(Board::Catan& model, GameClient& client, View& view);

    GameController(const GameController&) = delete;
    GameController operator=(const GameController&) = delete;

    void RunGame();


private:
    std::vector<std::unique_ptr<Handler>> handlers_;
    Board::Catan& gameModel_;
    View& gameView_;
    GameClient& gameClient_;
    int myTurn_;
    int currentTurn_ = 0;
};

} // namespace Controller

#endif