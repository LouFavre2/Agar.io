#include <SDL.h>
#include "ReglageJeu.hpp"

#pragma once
class Joueur {
public:
    Joueur();
    Joueur(double x, double y, double taille);
    
    ~Joueur();
    
    void deplace(bool pause);
    void incrementeVitesse(double varX, double varY);
    void changeDirectionVitesse(double nx, double ny);
    bool collision(const Joueur &joueur);
    void mange(Joueur &joueur);
    bool peutManger(const Joueur &joueur);
    void updateVitesse();
    float taille = 20.0;
    const double vittMax = 2.0;
    bool estVisible = true;
    bool estStatique = false;
    Uint8 r=255, g=255, b = 255;
    SDL_FRect corps = {ReglageJeu::LARGEUR_FENETRE/2.0,
                      ReglageJeu::HAUTEUR_FENETRE/2.0, taille, taille};
    SDL_FRect intersect = {0, 0, 0, 0};
    double vx = 1.111;
    double vy = 1.0;
    double vboost = 1.0;
    

};
