#include <iostream>
#include <thread>
#include <unistd.h>
#include <fstream>

#include "Board.h"
#include "GameController.h"
#include "GUI.h"
#include "random.h"

int View::qwe = 0;
std::mt19937 utility::Random::random_;

void seedRandom() {
    std::random_device rd;
    unsigned int x = rd();
    utility::Random::random_.seed(x);
    std::ofstream fout("RandomLog.txt", std::ios_base::app);
    fout << x << '\n';
}


int main() {
    seedRandom();
    View view{};   
    GameClient gameClient_;
    Board::Catan wow;
    
    Controller::GameController gc(wow, gameClient_, view);
    gc.RunGame();

    
    view.endGame();

    return 0;
}