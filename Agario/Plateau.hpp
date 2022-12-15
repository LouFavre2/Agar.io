#include <vector>
#include "ReglageJeu.hpp"
#include "Joueur.hpp"

#pragma once
class Plateau {
    
public:
    Plateau();
    ~Plateau();
    double fenetreXorigin = 0.0;
    double fenetreYorigin = 0.0;
    bool estJoueurDansFenetre(const Joueur &joueur);
    void draw(SDL_Renderer *);
    std::vector<Joueur> joueurs;
    void generationJoueurs(int nb);
    void ajoutStaticElem(int nb);
    void convertToFenetreCoords(double &x, double &y);
    void afficheJoueur(Joueur j, SDL_Renderer *renderer);
    
private:


};

