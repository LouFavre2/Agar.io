#include <SDL.h>
#include "Plateau.hpp"
#include "ReglageJeu.hpp"

#pragma once
class Jeu {
    
    
public:
    Jeu();
    ~Jeu();
    bool tick();
  
private:
    SDL_Window *win;
    SDL_Renderer *renderer;
    Plateau plateau;
    Joueur joueurPrincipal;
    bool pause;
    void buildMenu(SDL_Renderer* renderer, const char* state);
    void restart();
    int checkVictory();
  
};



