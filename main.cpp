#include <iostream>
#include <thread>
#include <unistd.h>
#include <fstream>

#include "Board.h"
#include "GameController.h"
#include "sdl.h"
#include "random.h"


int main() {
    std::cout << "Write two numbers:\n1) Command. 1 -- new game, 2 -- join game\n"
                 "2) Value. Number of players for new game and game id for join" << std::endl;
    int type, val;
    std::cin >> type >> val;

    utility::Random random;

    GUI::GUI view;
    GameClient gameClient_;
    Board::Catan wow(random);

    view.load_textures();
    view.roads = new GUI::Road_arr(view);
    view.buildings = new GUI::Building_arr(view);

    Controller::GameController gc(wow, gameClient_, view, random);

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