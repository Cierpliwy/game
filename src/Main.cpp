#include "Game.h"
#include "GameException.h"
#include <iostream>
#include <exception>
using namespace std;

int main(int, char *[])
{
    // Główna klasa związana w grą. W niej znajduje się cały kod gry.
    Game game;
    try {
        // Pierwszym krokiem jest inicjalizacja wszystkich bibliotek i 
        // danych używanych przez grę.
        game.initialize();
        // Gra w chodzi w główną pętlę zdarzeń. Funkcja run zostanie zakończona
        // po zamknięciu aplikacji.
        game.run();
        // Zamykanie bibliotek i czyszczenie danych.
        game.cleanup();
    } catch (GameException &e) {
        // Błędy krytyczne są zwracane jako wyjątki i upewniamy się, że
        // zostaną one zawsze obsłużone.
        cerr << e.what() << endl;
    } catch (exception &e) {
        cerr << e.what() << endl;
    }

    return 0;
}
