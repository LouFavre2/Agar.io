#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <SDL.h>
#include <SDL_ttf.h>
#include "Jeu.hpp"

int main(int argc, char *argv[])
{    
    if (TTF_Init() < 0) {
        std::cout << "Couldn't initialize TTF lib: " << TTF_GetError() << std::endl;
        return 1;
    }
    Jeu jeu;
    srand (time(NULL));
    bool is_running = true;
    while (is_running) {
        is_running = jeu.tick();
    }
    
    return EXIT_SUCCESS;
}

