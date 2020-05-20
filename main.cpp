#include <iostream>
#include <thread>
#include <unistd.h>
#include <fstream>

#include "Board.h"
#include "GameController.h"
#include "LocalServer.h"
#include "sdl.h"
#include "random.h"


namespace {


bool isGameLocal() {
    std::cout << "Please choose:\n1 -- local game\n2 -- game on server" << std::endl;
    int x;
    std::cin >> x;
    return x == 1;
}


auto getGameParams() {
    int action = 0;
    while (true) {
        std::cout << "1 --- start new game" << std::endl << "2 --- join game" << std::endl << "3 --- exit" << std::endl;
        std::cin >> action;
        if (action == 1) {
            std::cout << "How many people will play? 2, 3 or 4?" << std::endl;
            int players;
            std::cin >> players;
            if (players != 2 && players != 3 && players != 4) {
                std::cout << "Wrong number. Try again:" << std::endl;
                continue;
            }

            return std::make_pair(1, players);
        } else if (action == 2) {
            std::cout << "Type game id to join" << std::endl;
            int gameId;
            std::cin >> gameId;

            return std::make_pair(2, gameId);
        } else if (action == 3) {
            return std::make_pair(0, 0);
        } else {
            std::cout << "Wrong command. Try again:" << std::endl;
        }
    }
}


} // namespace


int main() {
    bool isLocal = isGameLocal();

    auto gameParams = getGameParams();
    if (gameParams == std::make_pair(0, 0)) {
        return 0;
    }

    LocalServer localServer;
    std::thread server(RunServer, &localServer, isLocal);

    GameClient gameClient_(isLocal);

    OrderInfo info = gameClient_.ConnectToGame(gameParams.first, gameParams.second);
    utility::Random random(info.seed());

    GUI::GUI view(info.id(), info.numberofplayers());
    Board::Catan wow(random, info.numberofplayers());
    view.robber = new GUI::Robber_arr(view);
    view.loadTextures(random, view);
    view.roads = new GUI::Road_arr();
    
    view.buildings = new GUI::Building_arr();
    view.resourses_img = new GUI::Resourses_arr();

    std::thread update(GUI::upgrade, &view);

    std::thread TimeTable(GUI::setTimeTable, &view);

    Controller::GameController gc(wow, gameClient_, view, random, info);

    std::thread music(GUI::playMusic, &view);

    gc.RunGame();

    music.join();
    update.join();
    TimeTable.join();

    localServer.terminate();
    server.join();

    return 0;
}