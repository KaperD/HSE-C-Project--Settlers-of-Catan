#include <memory>
#include <stdexcept>
#include <random>
#include <ctime>
#include <iostream>
#include <thread>
#include <chrono>
 
#include "GameController.h"
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
 
 
 
namespace {
 
bool roadIsSet = false;
bool villageIsSet = false;
 
} // namespace
 
 
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
    if (needSend) {

    }
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
    static_cast<void>(event);
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
    number1_ = diceInfo->number1();
    number2_ = diceInfo->number2();

    if (number1_ == 0) {
        number1_ = random_.getRandomNumberFromTo(1, 6);
        number2_ = random_.getRandomNumberFromTo(1, 6);
        diceInfo->set_number1(number1_);
        diceInfo->set_number2(number2_);
    }
 
    int numberSum = number1_ + number2_;
 
    if (numberSum <= 0 || numberSum > 12) {
        throw std::logic_error("Wrong dice number");
    }
 
    if (numberSum == 7) {
        //int hexNum = 0;
        // hexNum = запросить у View
        //gameModel_.setRobbers(hexNum);
        // изменить hexNum у View
    } else {
        gameModel_.giveResources(numberSum);
    }

    displayEvent(event);

    if (needSend) {
        sendEvent(event);
    }
}
 
void DiceHandler::displayEvent(Event& event) {
    static_cast<void>(event);
    gameView_.addDice(number1_, number2_);
    std::vector<int> v;
    const std::unordered_map<Board::Resource, int> m = gameModel_.getPlayerResources(static_cast<Board::PlayerNum>(myTurn_ + 1));
    for(auto e: m) {
        std::cout << e.second << std::endl;
        v.push_back(e.second);
    }
    std::cout << v.size() << std::endl;
    gameView_.updateResourses(v);
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
    static_cast<void>(event);
}
 
 
//===============BuildHandler===============
 
void BuildHandler::processEvent(Event& event, bool needSend) {
    if (event.type() != EventType::BUILD) {
        throw std::logic_error("Wrong type");
    }
 
 
    int Player = event.playerid() + 1;
    auto currentPlayer_ = static_cast<Board::PlayerNum>(Player);
    gameModel_.changeCurPlayer(currentPlayer_);
 
 
    buildingType_ = event.mutable_buildinfo()->buildingtype();
    x_ = event.mutable_buildinfo()->x();
    y_ = event.mutable_buildinfo()->y();
 
    auto type = static_cast<Board::BuildingType>(buildingType_);
    std::cout << x_ << ' ' << y_ << ' ' << static_cast<int>(Board::BuildingType::ROAD) << '\n';
    if (gameModel_.checkCards(type) && gameModel_.canBuild(type, x_, y_)) {
        std::cout << "YES" << '\n';
        gameModel_.settle(type, x_, y_);
        displayEvent(event);
        if (needSend) {
            sendEvent(event);
        }
    }
}
 
void BuildHandler::displayEvent(Event& event) {
    auto type = static_cast<Board::BuildingType>(buildingType_);
    int Player = event.playerid();
    if (type == Board::BuildingType::ROAD) {
        roadIsSet = true;
        gameView_.addRoad({x_, y_}, Player);
    }
    if (type == Board::BuildingType::VILLAGE) {
        villageIsSet = true;
        gameView_.addBuilding({x_, y_}, Player);
    }
    if (type == Board::BuildingType::CITY) {
        gameView_.addBuilding({x_, y_}, Player);
    }
    gameView_.updatePoints(gameModel_.Catan::getVictoryPoints());
    if (Player == myTurn_) {
        std::vector<int> v;
        const std::unordered_map<Board::Resource, int> m = gameModel_.getPlayerResources(static_cast<Board::PlayerNum>(Player + 1));
        std::cout << "Player " << Player << "resources" << std::endl;
        for(auto e: m) {
            std::cout << e.second << std::endl;
            v.push_back(e.second);
        }
        std::cout << v.size() << std::endl;
        gameView_.updateResourses(v);
    }
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
    static_cast<void>(event);
}
 
 
 
//===============NextPhaseHandler===============
 
void NextPhaseHandler::processEvent(Event& event, bool needSend) {
    if (event.type() != EventType::NEXTPHASE) {
        throw std::logic_error("Wrong type");
    }
    displayEvent(event);
    if (needSend) {

    }
}
 
void NextPhaseHandler::displayEvent(Event& event) {
    //gameView_.update();
    static_cast<void>(event);
}
 
 
 
//===============EndGameHandler===============
 
void EndGameHandler::processEvent(Event& event, bool needSend) {
    if (event.type() != EventType::ENDGAME) {
        throw std::logic_error("Wrong type");
    }
    displayEvent(event);
    if (needSend) {
        sendEvent(event);
    }
}
 
void EndGameHandler::displayEvent(Event& event) {
    gameView_.quit.store(true);
    static_cast<void>(event);
}
 
 
 
//===============GameController===============
 
 
GameController::GameController(Board::Catan& model, GameClient& client, GUI::GUI& view, utility::Random& ran, const OrderInfo& info)
    : gameModel_(model)
    , gameView_(view)
    , gameClient_(client)
    , myTurn_(info.id())
    , numberOfPlayers_(info.numberofplayers()) {
    std::cout << "My turn is " << myTurn_ << ", numOfPl " << numberOfPlayers_ << std::endl;
 
    for (int k = 0; k < 7; ++k) {
        handlers_.push_back(nullptr);
    }
    handlers_[0] = std::make_unique<CardHandler     >(gameModel_, gameView_, gameClient_);
    handlers_[1] = std::make_unique<DiceHandler     >(gameModel_, gameView_, gameClient_, ran, myTurn_);
    handlers_[2] = std::make_unique<MarketHandler   >(gameModel_, gameView_, gameClient_);
    handlers_[3] = std::make_unique<BuildHandler    >(gameModel_, gameView_, gameClient_, myTurn_);
    handlers_[4] = std::make_unique<EndTurnHandler  >(gameModel_, gameView_, gameClient_);
    handlers_[5] = std::make_unique<NextPhaseHandler>(gameModel_, gameView_, gameClient_);
    handlers_[6] = std::make_unique<EndGameHandler  >(gameModel_, gameView_, gameClient_);
}
 
 
void GameController::RunGame() {
    std::cout << myTurn_ << std::endl;

    bool quit = false;
    while (!quit) {
        if (currentTurn_ == myTurn_) {
            while (true) {
                Event event = gameView_.getEvent();
                int x = event.type();
                event.set_playerid(myTurn_);
                handlers_[x]->processEvent(event, true);
                if (gameModel_.isFinished()) {
                    Event end;
                    end.set_type(EventType::ENDGAME);
                    gameClient_.SendEvent(end);
                    gameView_.quit.store(true);
                    quit = true;
                    break;
                }
                if (x == EventType::ENDGAME) {
                    quit = true;
                    break;
                } else if (x == EventType::ENDTURN) {
                    break;
                }
            }
        } else {
            while (true) {
                if (!gameClient_.HasEvent()) {
                    std::cout << "I'm sleeping" << std::endl;
                    std::this_thread::sleep_for(std::chrono::seconds(1));
                    continue;
                }
                Event event = gameClient_.GetEvent();
                int x = event.type();
                handlers_[x]->processEvent(event, false);
                if (gameModel_.isFinished()) {
                    gameView_.quit.store(true);
                    quit = true;
                    break;
                }
                if (x == EventType::ENDGAME) {
                    quit = true;
                    break;
                } else if (x == EventType::ENDTURN) {
                    break;
                }
            }
        }
        ++currentTurn_;
        currentTurn_ %= numberOfPlayers_;
    }
}
 
 
void GameController::BeginGame() {
    for (int turn = 0; turn < numberOfPlayers_ * 2; ++turn) {
        if (turn >= numberOfPlayers_) {
            currentTurn_ = 2 * numberOfPlayers_ - turn - 1;
        } else {
            currentTurn_ = turn;
        }
 
        if (currentTurn_ == myTurn_) {
            while (true) {
                Event event = gameView_.ThirdStage();
                int x = event.type();
                if (x == EventType::BUILD) {
                    handlers_[x]->processEvent(event, true);
                } else if (x == EventType::ENDTURN) {
                    if (roadIsSet && villageIsSet) {
                        roadIsSet = false;
                        villageIsSet = false;
                        handlers_[x]->processEvent(event, true);
                        break;
                    }
                } else if (x == EventType::ENDGAME) {
                    handlers_[x]->processEvent(event, true);
                    break;
                }
            }
        } else {
            while (true) {
                if (!gameClient_.HasEvent()) {
                    std::cout << "I'm sleeping" << std::endl;
                    std::this_thread::sleep_for(std::chrono::seconds(1));
                    continue;
                }
                Event event = gameClient_.GetEvent();
                int x = event.type();
                if (x == EventType::BUILD) {
                    handlers_[x]->processEvent(event, false);
                } else if (x == EventType::ENDTURN) {
                    if (roadIsSet && villageIsSet) {
                        handlers_[x]->processEvent(event, false);
                        break;
                    }
                } else if (x == EventType::ENDGAME) {
                    handlers_[x]->processEvent(event, false);
                    break;
                }
            }
        }
    }
}
 
 
} // namespace Controller
