#include <iostream>
#include <thread>
#include <unistd.h>
#include <fstream>

#include "Board.h"
#include "GameController.h"
#include "sdl.h"
#include "random.h"

std::mt19937 utility::Random::random_;

void seedRandom() {
    std::random_device rd;
    unsigned int x = rd();
    utility::Random::random_.seed(x);
    std::ofstream fout("RandomLog.txt", std::ios_base::app);
    fout << x << '\n';
    for (int k = 0; k < 10; ++k) {
        std::cout << utility::Random::getRandomNumberFromTo(1, 6) << std::endl;
    }
}


int main() {
    //seedRandom();
    std::cout << "Write two numbers:\n1) Command. 1 -- new game, 2 -- join game\n"
                 "2) Value. Number of players for new game and game id for join" << std::endl;
    int type, val;
    std::cin >> type >> val;
    GUI::GUI view;
    GameClient gameClient_;
    Board::Catan wow;

    view.load_textures();
    view.roads = new GUI::Road_arr(view);
    view.buildings = new GUI::Building_arr(view);

    Controller::GameController gc(wow, gameClient_, view);

    if (!gc.ConnectToGame(type, val)) {
        return 0;
    }
    std::thread update(GUI::upgrade, &view);

    //std::thread music(play_music, a);

    gc.RunGame();

//    music.join();
    update.join();

    return 0;
}