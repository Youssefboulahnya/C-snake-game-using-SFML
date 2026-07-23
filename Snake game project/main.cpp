// main.cpp
#include "game.h"
#include <iostream>

int main() {
    try {
        SnakeGame game;
        game.run();
    } catch (const std::exception &e) {
        std::cerr << "Erreur: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
