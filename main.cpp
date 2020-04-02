#include <iostream>
#include <thread>
#include <unistd.h>

#include "Board.h"
#include "GameController.h"
#include "GUI.h"

int View::qwe = 0;

int main() {
    View view{};   
    GameClient gameClient_;
    Board::Catan wow;
    
    Controller::GameController gc(wow, gameClient_, view);
    gc.RunGame();

    
    view.endGame();

    return 0;
}