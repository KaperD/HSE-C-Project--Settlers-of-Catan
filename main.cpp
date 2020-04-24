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
    GUI::GUI view;
    GameClient gameClient_;
    Board::Catan wow;

    view.load_textures();
    view.roads = new GUI::Road_arr(view);
    view.buildings = new GUI::Building_arr(view);

    std::thread update(GUI::upgrade, &view);
    //std::thread music(play_music, a);

    view.getTurn();
    Controller::GameController gc(wow, gameClient_, view);
    gc.RunGame();

//    music.join();
    update.join();

    return 0;
}