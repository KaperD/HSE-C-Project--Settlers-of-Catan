#include <stdexcept>
#include <random>
#include <ctime>
#include <iostream>

#include "GameController.h"

using game::EventType;
using game::Void;
using game::OrderInfo;
using game::Event;
using game::Card;
using game::Dice;
using game::Market;
using game::Build;
using game::Player;
using game::Network;

namespace Controller {

namespace {
    std::mt19937 random(time(0));
    std::uniform_int_distribution<int> randomDiceNumber(1, 6); 
}


//===============Handler===============

void Handler::sendEvent(::game::Event& event) {
    gameClient_.SendEvent(event);
}



//===============CardHandler===============

void CardHandler::processEvent(Event& event, bool needSend) {
    if (event.type() != EventType::CARD) {
        throw std::logic_error("Wrong type");
    }

    currentPlayer_ = event.playerid();
    cardType_ = event.cardinfo().cardtype();
    /*
    if (gameModel_.hasCard(currentPlayer_, cardType_)) {
        gameModel_.playCard(currentPlayer_, cardType_);
        displayEvent(event);
        sendEvent(event);
    } else {
        Возможно сообщить игроку о том, что у него нет карты, но лучше просто не допустить такого.
    }
    */
}

void CardHandler::displayEvent(Event& event) {
    // TODO
}



//===============DiceHandler===============

void DiceHandler::processEvent(Event& event, bool needSend) {
    if (event.type() != EventType::DICE) {
        throw std::logic_error("Wrong type");
    }

    currentPlayer_ = event.playerid();
    auto diceInfo = event.mutable_diceinfo();
    number_ = diceInfo->number();

    if (number_== 0) {
        number_ = randomDiceNumber(random) + randomDiceNumber(random);
        diceInfo->set_number(number_);
    }

    if (number_ <= 0 || number_ > 12) {
        throw std::logic_error("Wrong dice number");
    }

    /*
    gameModel_.giveResourses...(...)
    displayEvent(event);
    sendEvent(event);
    */
}

void DiceHandler::displayEvent(Event& event) {
    /*
    Вывести изменения ресурсов и выпавшее число
    */
}



//===============MarketHandler===============

void MarketHandler::processEvent(Event& event, bool needSend) {
    if (event.type() != EventType::MARKET) {
        throw std::logic_error("Wrong type");
    }

    currentPlayer_ = event.playerid();
    requiredResource_ = event.mutable_marketinfo()->requiredresource();
    ownedResource_ = event.mutable_marketinfo()->ownedresource();

    /*
    if (gameModel.hasResourse(currentPlayer_, ownedResource_)) {
        gameModel.swapresourse(currentPlayer_, ownedResource_, requiredResource_);
        displayEvent(event);
        sendEvent(event);
    } else {
        // сказать о том, что не достаточно ресурсов
    }
    */

}

void MarketHandler::displayEvent(Event& event) {
    /*
    Вывести изменения ресурсов
    */
}



//===============BuildHandler===============

void BuildHandler::processEvent(Event& event, bool needSend) {
    if (event.type() != EventType::BUILD) {
        throw std::logic_error("Wrong type");
    }

    currentPlayer_ = event.playerid();
    buildingType_ = event.mutable_buildinfo()->buildingtype();
    x_ = event.mutable_buildinfo()->x();
    y_ = event.mutable_buildinfo()->y();

    displayEvent(event);
    if (needSend) {
        sendEvent(event);
    }
    /*
    if (gameModel.canBuild(buildingType_, currentPlayer_, x_, y_)) {
        gameModel.build(buildingType_, currentPlayer_, x_, y_);
        displayEvent(event);
        sendEvent(event);
    }
    */
}

void BuildHandler::displayEvent(Event& event) {
    gameView_.build(x_, y_);
    /*
    Построить, обновить очки игрока
    */
}



//===============EndTurnHandler===============

void EndTurnHandler::processEvent(Event& event, bool needSend) {
    if (event.type() != EventType::ENDTURN) {
        throw std::logic_error("Wrong type");
    }
    //displayEvent(event);
    if (needSend) {
        sendEvent(event);
    }
}

void EndTurnHandler::displayEvent(Event& event) {
    /*
    Убрать некоторый интерфейс
    */
}



//===============NextPhaseHandler===============

void NextPhaseHandler::processEvent(Event& event, bool needSend) {
    if (event.type() != EventType::NEXTPHASE) {
        throw std::logic_error("Wrong type");
    }
    displayEvent(event);
    if (needSend) {
        sendEvent(event);
    }
}

void NextPhaseHandler::displayEvent(Event& event) {
    //gameView_.update();
    /*
    Отобразить новые кнопки
    */
}



//===============GameController===============

GameController::GameController(Board::Catan& model, GameClient& client, View& view)
    : gameModel_(model)
    , gameClient_(client) 
    , gameView_(view) {
    for (int k = 0; k < 7; ++k) {
        handlers_.push_back(nullptr);
    }
    handlers_[3] = std::unique_ptr<BuildHandler>(new BuildHandler(gameModel_, gameView_, gameClient_));
    handlers_[4] = std::unique_ptr<EndTurnHandler>(new EndTurnHandler(gameModel_, gameView_, gameClient_));
    handlers_[5] = std::unique_ptr<NextPhaseHandler>(new NextPhaseHandler(gameModel_, gameView_, gameClient_));
}

void GameController::RunGame() {
    game::OrderInfo info = gameClient_.Register();
    myTurn_ = info.id();
    myTurn_ %= 2;
    std::cout << myTurn_ << std::endl;

    bool quit = false;
    while (!quit) {
        if (currentTurn_ == myTurn_) {
            while (true) {
                Event event = gameView_.getTurn();
                event.set_playerid(myTurn_);
                int x = event.type();
                std::cout << x << std::endl;
                handlers_[x]->processEvent(event, true);
                if (x == EventType::ENDTURN) {
                    quit = true;
                    break;
                } else if (x == EventType::NEXTPHASE) {
                    break;
                }
            }
        } else {
            while (true) {
                Event event = gameClient_.GetEvent();
                int x = event.type();
                std::cout << x << std::endl;
                handlers_[x]->processEvent(event, false);
                if (x == EventType::ENDTURN) {
                    quit = true;
                    break;
                } else if (x == EventType::NEXTPHASE) {
                    break;
                }
            }
        }

        ++currentTurn_;
        currentTurn_ %= 2;
    }

}


// private:
//     std::vector<std::unique_ptr<Handler>> handlers_;
//     Board::Catan gameModel_;
//     // View m_;
//     GameClient gameClient_;
//     int myTurn_;
//     int currentTurn_;
// };

} // namespace Controller