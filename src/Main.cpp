#include "Game.h"
#include "GameException.h"
#include <iostream>
#include <exception>
using namespace std;

int main(int, char *[])
{
    Game game;
    try {
        game.initialize();
        game.run();
        game.cleanup();
    } catch (GameException &e) {
        cerr << e.what() << endl;
    } catch (exception &e) {
        cerr << e.what() << endl;
    }

    return 0;
}
