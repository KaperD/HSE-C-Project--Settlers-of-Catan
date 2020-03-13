#include <iostream>
#include "Board.h"
#include "GameController.h"

int gui (int argc, char ** args);

int main(int argc, char ** args) {
    gui(argc, args);
    GameClient gameClient_;
    ::game::Event ev;
    ev.set_type((::game::EventType)2);
    ev.set_playerid(1);
    gameClient_.SendEvent(ev);
    Catan wow;
    std::cout << "Second Compile!\n";
    std::cout << wow.canBuild(BuildingType::SETTLEMENT, PlayerNum::GAMER1, 2, 12) << '\n';
    return 0;
}