#include <memory>
#include <stdexcept>
#include <random>
#include <ctime>
#include <iostream>
#include <thread>

#include "GameController.h"
#include "EventQueue.h"
#include "random.h"

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

using utility::Random;

namespace Controller {

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

    int Player = event.playerid() + 1;
    auto currentPlayer_ = static_cast<Board::PlayerNum>(Player);
    gameModel_.changeCurPlayer(currentPlayer_);

    auto diceInfo = event.mutable_diceinfo();
    number_ = diceInfo->number();

    if (number_== 0) {
        number_ = Random::getRandomNumberFromTo(1, 6) + Random::getRandomNumberFromTo(1, 6);
        diceInfo->set_number(number_);
    }

    if (number_ <= 0 || number_ > 12) {
        throw std::logic_error("Wrong dice number");
    }

    if (number_ == 7) {
        int hexNum = 0;
        // hexNum = запросить у View
        gameModel_.setRobbers(hexNum);
        // изменить hexNum у View
    } else {
        gameModel_.giveResources(number_);
    }
    displayEvent(event);
    if (needSend) {
        sendEvent(event);
    }
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

    int Player = event.playerid() + 1;
    auto currentPlayer_ = static_cast<Board::PlayerNum>(Player);
    requiredResource_ = event.mutable_marketinfo()->requiredresource();
    ownedResource_ = event.mutable_marketinfo()->ownedresource();

    gameModel_.changeCurPlayer(currentPlayer_);

    auto reTo = static_cast<Board::Resource>(requiredResource_);
    auto reFrom = static_cast<Board::Resource>(ownedResource_);
    
    if (gameModel_.trade(reFrom, reTo)) {
        displayEvent(event);
        if (needSend) {
            sendEvent(event);
        }
    } else {
        // сказать о том, что не достаточно ресурсов
    }
    

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

    int Player = event.playerid() + 1;
    auto currentPlayer_ = static_cast<Board::PlayerNum>(Player);
    buildingType_ = event.mutable_buildinfo()->buildingtype();
    x_ = event.mutable_buildinfo()->x();
    y_ = event.mutable_buildinfo()->y();

    gameModel_.changeCurPlayer(currentPlayer_);
    displayEvent(event);
    if (needSend) {
        sendEvent(event);
    }

    //auto type = static_cast<Board::BuildingType>(buildingType_);

    // if (gameModel_.canBuild(type, x_, y_)) {
    //     gameModel_.settle(type, x_, y_);
    //     displayEvent(event);
    //     if (needSend) {
    //         sendEvent(event);
    //     }
    // }
    
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
    displayEvent(event);
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
}

void NextPhaseHandler::displayEvent(Event& event) {
    //gameView_.update();
    /*
    Отобразить новые кнопки
    */
}



//===============EndGameHandler===============

void EndGameHandler::processEvent(Event& event, bool needSend) {
    if (event.type() != EventType::ENDGAME) {
        throw std::logic_error("Wrong type");
    }
    sendEvent(event);
}

void EndGameHandler::displayEvent(Event& event) {
}



//===============GameController===============


namespace {

utility::EventQueue events_;

void getEventsFromServer(GameClient* gameClient_) {
    while (true) {
        Event event = gameClient_->GetEvent();
        events_.push(event);
        if (event.type() == EventType::ENDGAME) {
            break;
        }
    }
}
    
} // namespace



GameController::GameController(Board::Catan& model, GameClient& client, View& view)
    : gameModel_(model)
    , gameView_(view) 
    , gameClient_(client) {

    game::OrderInfo info = gameClient_.Register();
    myTurn_ = info.id();
    myTurn_ %= 2;

    for (int k = 0; k < 7; ++k) {
        handlers_.push_back(nullptr);
    }
    handlers_[0] = std::make_unique<CardHandler>(gameModel_, gameView_, gameClient_);
    handlers_[1] = std::make_unique<DiceHandler>(gameModel_, gameView_, gameClient_);
    handlers_[2] = std::make_unique<MarketHandler>(gameModel_, gameView_, gameClient_);
    handlers_[3] = std::make_unique<BuildHandler>(gameModel_, gameView_, gameClient_);
    handlers_[4] = std::make_unique<EndTurnHandler>(gameModel_, gameView_, gameClient_);
    handlers_[5] = std::make_unique<NextPhaseHandler>(gameModel_, gameView_, gameClient_);
    handlers_[6] = std::make_unique<EndGameHandler>(gameModel_, gameView_, gameClient_);
}

void GameController::RunGame() {
    std::cout << myTurn_ << std::endl;
    std::thread serverEvents(getEventsFromServer, &gameClient_);

    bool quit = false;
    while (!quit) {
        if (currentTurn_ == myTurn_) {
            while (true) {
                // start() Засекается время начала
                Event event = gameView_.getTurn();
                int x = event.type();
                handlers_[x]->processEvent(event, true);
                if (gameModel_.isFinished()) {
                    Event end;
                    end.set_type(EventType::ENDGAME);
                    gameClient_.SendEvent(end);
                    quit = true;
                    break;
                }
                if (x == EventType::ENDGAME) {
                    quit = true;
                    break;
                } else if (x == EventType::ENDTURN) {
                    break;
                }
                // update() // вывести текущее состояние
                // delay() // подождать, если действия выполнелись слишком быстро
            }
        } else {
            while (true) {
                // start() Засекается время начала
                if (!events_.empty()) {
                    Event event = events_.front();
                    int x = event.type();
                    handlers_[x]->processEvent(event, false);
                    if (x == EventType::ENDGAME) {
                        quit = true;
                        break;
                    } else if (x == EventType::ENDTURN) {
                        break;
                    }
                }
                // update() // вывести текущее состояние
                // delay() // подождать, если действия выполнелись слишком быстро
            }
        }
        ++currentTurn_;
        currentTurn_ %= 2;
    }
    serverEvents.join();
}

} // namespace Controller