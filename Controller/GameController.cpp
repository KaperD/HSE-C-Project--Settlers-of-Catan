#include <memory>
#include <stdexcept>
#include <random>
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

    int Player = event.playerid() + 1;
    auto currentPlayer_ = static_cast<Board::PlayerNum>(Player);
    gameModel_.changeCurPlayer(currentPlayer_);

    cardType_ = event.cardinfo().cardtype();
    auto modelCardType = static_cast<Board::DevelopmentCard>(cardType_);

    auto cards = gameModel_.getPlayerDevCards(currentPlayer_);

    if (cards[modelCardType] > 0) {
        if (Player - 1 == myTurn_) {
            event.mutable_cardinfo()->set_extradata(0);
            if (modelCardType == Board::DevelopmentCard::KNIGHT) {
                int hexNum = gameView_.getCoorsRobber(gameView_);
                gameView_.setRobber(hexNum);
                event.mutable_cardinfo()->set_extradata(hexNum);
            } else if (modelCardType == Board::DevelopmentCard::MONOPOLY) {
                int resource = gameView_.getCoorsResoursesCards();
                event.mutable_cardinfo()->set_extradata(resource + 1);
            } else {
                event.mutable_cardinfo()->set_extradata(0);
            }
        }

        int extraData = event.mutable_cardinfo()->extradata();
        gameModel_.playDevCard(modelCardType, extraData);

        displayEvent(event);
        if (needSend) {
            sendEvent(event);
        }
    } else {
        //Возможно сообщить игроку о том, что у него нет карты, но лучше просто не допустить такого.
    }

}
 
void CardHandler::displayEvent(Event& event) {
    static_cast<void>(event);
    gameView_.updatePoints(gameModel_.Catan::getVictoryPoints());
    std::vector<int> v;
    auto m = gameModel_.getPlayerResources(static_cast<Board::PlayerNum>(myTurn_ + 1));
    v.push_back(m[Board::Resource::WOOL]);
    v.push_back(m[Board::Resource::ORE]);
    v.push_back(m[Board::Resource::CLAY]);
    v.push_back(m[Board::Resource::TREE]);
    v.push_back(m[Board::Resource::WHEAT]);
    gameView_.updateResourses(v);

    auto m2 = gameModel_.getPlayerDevCards(static_cast<Board::PlayerNum>(myTurn_ + 1));
    std::vector<bool> cards;
    cards.push_back(m2[Board::DevelopmentCard::KNIGHT] != 0);
    cards.push_back(m2[Board::DevelopmentCard::VICTORY_POINT] != 0);
    cards.push_back(m2[Board::DevelopmentCard::ROAD_BUILDING] != 0);
    cards.push_back(m2[Board::DevelopmentCard::MONOPOLY] != 0);
    cards.push_back(m2[Board::DevelopmentCard::INVENTION] != 0);
    gameView_.updateDevCards(cards);
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

    if (Player - 1 == myTurn_) {
        number1_ = random_.getRandomNumberFromTo(1, 6);
        number2_ = random_.getRandomNumberFromTo(1, 6);
        diceInfo->set_number1(number1_);
        diceInfo->set_number2(number2_);
    }
 
    int numberSum = number1_ + number2_;

    if (numberSum == 7) {
        gameView_.addDice(number1_, number2_);
        int hexNum = 0;
        if (Player - 1 == myTurn_) {
            hexNum = gameView_.getCoorsRobber(gameView_);
            diceInfo->set_hexnumber(hexNum);
        } else {
            hexNum = diceInfo->hexnumber();
        }
        gameView_.setRobber(hexNum);
        gameModel_.setRobbers(hexNum);
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
    auto m = gameModel_.getPlayerResources(static_cast<Board::PlayerNum>(myTurn_ + 1));
    v.push_back(m[Board::Resource::WOOL]);
    v.push_back(m[Board::Resource::ORE]);
    v.push_back(m[Board::Resource::CLAY]);
    v.push_back(m[Board::Resource::TREE]);
    v.push_back(m[Board::Resource::WHEAT]);
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
 
    auto reTo = static_cast<Board::Resource>(++requiredResource_);
    auto reFrom = static_cast<Board::Resource>(++ownedResource_);
   
    if (gameModel_.trade(reFrom, reTo)) {
        if (needSend) {
            sendEvent(event);
        }
    } else if (Player == myTurn_ + 1) {
        gameView_.setTable(2);
    }

    displayEvent(event);

}
 
void MarketHandler::displayEvent(Event& event) {
    // TODO: можно вывести сообщение о том, что обмен невозможен
    int Player = event.playerid();
    if (Player == myTurn_) {
        std::vector<int> v;
        auto m = gameModel_.getPlayerResources(static_cast<Board::PlayerNum>(Player + 1));
        v.push_back(m[Board::Resource::WOOL]);
        v.push_back(m[Board::Resource::ORE]);
        v.push_back(m[Board::Resource::CLAY]);
        v.push_back(m[Board::Resource::TREE]);
        v.push_back(m[Board::Resource::WHEAT]);
        gameView_.updateResourses(v);
    } // TODO: можно вывести сообщение о том, что такой-то игрок поменял такой-то ресурс на такой-то
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

    if (type == Board::BuildingType::DevCard) {
        if (gameModel_.buildDevCard() != Board::DevelopmentCard::NONE) {
            displayEvent(event);
            if (needSend) {
                sendEvent(event);
            }
        } else if (Player == myTurn_ + 1) {
            gameView_.setTable(2);
        }
    } else {
        if (gameModel_.checkCards(type) && gameModel_.canBuild(type, x_, y_)) {
            gameModel_.settle(type, x_, y_);
            displayEvent(event);
            if (needSend) {
                sendEvent(event);
            }
        } else if (!gameModel_.checkCards(type)) {
            if (Player == myTurn_ + 1) gameView_.setTable(2);
        } else if (!gameModel_.canBuild(type, x_, y_)){
            if (Player == myTurn_ + 1) gameView_.setTable(0);
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
        auto m = gameModel_.getPlayerResources(static_cast<Board::PlayerNum>(Player + 1));
        v.push_back(m[Board::Resource::WOOL]);
        v.push_back(m[Board::Resource::ORE]);
        v.push_back(m[Board::Resource::CLAY]);
        v.push_back(m[Board::Resource::TREE]);
        v.push_back(m[Board::Resource::WHEAT]);
        gameView_.updateResourses(v);


        auto m1 = gameModel_.getPlayerDevCards(static_cast<Board::PlayerNum>(Player + 1));
        std::vector<bool> cards;
        cards.push_back(m1[Board::DevelopmentCard::KNIGHT] != 0);
        cards.push_back(m1[Board::DevelopmentCard::VICTORY_POINT] != 0);

        cards.push_back(m1[Board::DevelopmentCard::ROAD_BUILDING] != 0);
        cards.push_back(m1[Board::DevelopmentCard::MONOPOLY] != 0);
        cards.push_back(m1[Board::DevelopmentCard::INVENTION] != 0);
        gameView_.updateDevCards(cards);
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
    // TODO: желательно вывести пустой интерфейс
    static_cast<void>(event);
}
 
 
 
//===============NextPhaseHandler===============
 
void NextPhaseHandler::processEvent(Event& event, bool needSend) {
    static_cast<void>(needSend);
    if (event.type() != EventType::NEXTPHASE) {
        throw std::logic_error("Wrong type");
    }
    displayEvent(event);
}
 
void NextPhaseHandler::displayEvent(Event& event) {
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
 
    for (int k = 0; k < 7; ++k) {
        handlers_.push_back(nullptr);
    }
    handlers_[0] = std::make_unique<CardHandler     >(gameModel_, gameView_, gameClient_, myTurn_);
    handlers_[1] = std::make_unique<DiceHandler     >(gameModel_, gameView_, gameClient_, ran, myTurn_);
    handlers_[2] = std::make_unique<MarketHandler   >(gameModel_, gameView_, gameClient_, myTurn_);
    handlers_[3] = std::make_unique<BuildHandler    >(gameModel_, gameView_, gameClient_, myTurn_);
    handlers_[4] = std::make_unique<EndTurnHandler  >(gameModel_, gameView_, gameClient_);
    handlers_[5] = std::make_unique<NextPhaseHandler>(gameModel_, gameView_, gameClient_);
    handlers_[6] = std::make_unique<EndGameHandler  >(gameModel_, gameView_, gameClient_);
}
 
 
void GameController::RunGame() {
     if (BeginGame()) {
         return;
     }


    gameModel_.gotoNextGamePhase();
    bool quit = false;
    while (!quit) {
        gameView_.updatePlayer(currentTurn_);

        if (currentTurn_ == myTurn_) {
            while (true) {
                Event event = gameView_.getEvent();
                int x = event.type();
                event.set_playerid(myTurn_);
                handlers_[x]->processEvent(event, true);
                if (gameModel_.isFinished()) {
                    // TODO: вывести победителя
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
                    std::this_thread::sleep_for(std::chrono::seconds(1));
                    continue;
                }
                Event event = gameClient_.GetEvent();
                int x = event.type();
                handlers_[x]->processEvent(event, false);
                if (gameModel_.isFinished()) {
                    // TODO: вывести победителя
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


bool GameController::BeginGame() {
    for (int turn = 0; turn < numberOfPlayers_ * 2; ++turn) {
        if (turn >= numberOfPlayers_) {
            currentTurn_ = 2 * numberOfPlayers_ - turn - 1;
        } else {
            currentTurn_ = turn;
        }
        gameView_.updatePlayer(currentTurn_);
        roadIsSet = false;
        villageIsSet = false;
        if (currentTurn_ == myTurn_) {
            while (true) {
                Event event = gameView_.ThirdStage();
                int x = event.type();
                event.set_playerid(myTurn_);
                if (x == EventType::BUILD) {
                    auto type = static_cast<Board::BuildingType>(event.mutable_buildinfo()->buildingtype());
                    if (type == Board::BuildingType::ROAD) {
                        if (!roadIsSet) {
                            handlers_[x]->processEvent(event, true);
                        } else {
                            gameView_.setTable(0);
                        }
                    }
                    if (type == Board::BuildingType::VILLAGE) {
                        if (!villageIsSet) {
                            handlers_[x]->processEvent(event, true);
                        } else {
                            gameView_.setTable(0);
                        }
                    }
                } else if (x == EventType::ENDTURN) {
                    if (roadIsSet && villageIsSet) {
                        handlers_[x]->processEvent(event, true);
                        break;
                    } else {
                        gameView_.setTable(3);
                    }
                } else if (x == EventType::ENDGAME) {
                    handlers_[x]->processEvent(event, true);
                    return true;
                }
            }
        } else {
            while (true) {
                if (!gameClient_.HasEvent()) {
                    std::this_thread::sleep_for(std::chrono::seconds(1));
                    continue;
                }
                Event event = gameClient_.GetEvent();
                int x = event.type();
                if (x == EventType::BUILD) {
                    handlers_[x]->processEvent(event, false);
                } else if (x == EventType::ENDTURN) {
                    handlers_[x]->processEvent(event, false);
                    break;
                } else if (x == EventType::ENDGAME) {
                    handlers_[x]->processEvent(event, false);
                    return true;
                }
            }
        }
    }
    return false;
}
 
 
} // namespace Controller
