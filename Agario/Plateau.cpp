#include <SDL.h>
#include "Plateau.hpp"
#include "Jeu.hpp"

void fillCircle(SDL_Renderer* renderer, int x, int y, int radius, SDL_Color color);

Plateau::Plateau(){
}


Plateau::~Plateau(){
}

void Plateau::draw(SDL_Renderer *renderer){
    
}
  

bool
Plateau::estJoueurDansFenetre(const Joueur &joueur)
{
    return joueur.corps.x > fenetreXorigin
    && joueur.corps.y > fenetreYorigin
    && joueur.corps.x < fenetreXorigin + ReglageJeu::LARGEUR_FENETRE
    && joueur.corps.y < fenetreYorigin + ReglageJeu::HAUTEUR_FENETRE;
}


void
Plateau::ajoutStaticElem(int nb){
    for (int i=0; i < nb; i++){
          double x  = (double) (rand()%ReglageJeu::LARGEUR_MONDE_JEU);
          double y  = (double) (rand()%ReglageJeu::HAUTEUR_MONDE_JEU);
          double t  = 5.0;
          double vx  = 0.0;
          double vy = 0.0;
          Joueur j;
          j.estStatique = true;
          joueurs.push_back(j);
          Joueur &jj = joueurs.at(joueurs.size()-1);
          jj.vx = vx;
          jj.vy = vy;
          jj.corps.x = x;
          jj.corps.y = y;
          jj.corps.w = t;
          jj.corps.h = t;

          jj.r = 128.0;
          jj.g = 0.0;
          jj.b = 0.0;
          jj.vy = 0.0;
          jj.vy = 0.0;
          
      }
}


void
Plateau::generationJoueurs(int nb)
{
    joueurs.clear();
    for (int i=0; i < nb; i++){
        double x  = (double) (rand()%ReglageJeu::LARGEUR_MONDE_JEU);
        double y  = (double) (rand()%ReglageJeu::HAUTEUR_MONDE_JEU);
        double t  = (double) (rand()%30);

        Uint8 r  = rand()%255;
        Uint8 g  = rand()%255;
        Uint8 b  = rand()%255;
        double vx  = (double) ((rand()%4000)/1000.0) * (rand()%2 == 0 ? 1.0 : -1.0);
        double vy = (double) ((rand()%4000)/1000.0) * (rand()%2 == 0 ? 1.0 : -1.0);
        Joueur j;
        joueurs.push_back(j);
        Joueur &jj = joueurs.at(joueurs.size()-1);
        jj.vx = vx;
        jj.vy = vy;
        jj.corps.x = x;
        jj.corps.y = y;
        jj.corps.w = t;
        jj.corps.h = t;

        jj.r = r;
        jj.g = g;
        jj.b = b;
        jj.vy = vx;
        jj.vy = vy;
        jj.updateVitesse();
        
    }
}

void
Plateau::convertToFenetreCoords(double &x, double &y)
{
    x = x - fenetreXorigin;
    y = y - fenetreYorigin;
}


void
Plateau::afficheJoueur(Joueur j, SDL_Renderer *renderer){
    if (j.estVisible){
        double xx = j.corps.x, yy = j.corps.y;
        convertToFenetreCoords(xx, yy);
        SDL_Color c = { j.r, j.g, j.b};
        fillCircle(renderer, xx, yy, (int)j.corps.w/2, c );
    }
}

void fillCircle(SDL_Renderer* renderer, int x, int y, int radius, SDL_Color color)
{
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, 255);
    for (int w = 0; w < radius * 2; w++)
    {
        for (int h = 0; h < radius * 2; h++)
        {
            int dx = radius - w;
            int dy = radius - h;
            if ((dx * dx + dy * dy) <= (radius * radius))
            {
                SDL_RenderDrawPoint(renderer, x + dx, y + dy);
            }
        }
    }
}