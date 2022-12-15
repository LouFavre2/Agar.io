#include <SDL.h>
#include <SDL_ttf.h>
#include <iostream>
#include "Jeu.hpp"
#include "ReglageJeu.hpp"


Jeu::Jeu(): joueurPrincipal(ReglageJeu::LARGEUR_FENETRE/2.0, ReglageJeu::HAUTEUR_FENETRE/2.0, 30.0)
{
    SDL_Init(SDL_INIT_EVERYTHING);
    win = SDL_CreateWindow("AgarIO CPP - Lou Tom",
                           SDL_WINDOWPOS_CENTERED,
                           SDL_WINDOWPOS_CENTERED,
                           ReglageJeu::LARGEUR_FENETRE, ReglageJeu::HAUTEUR_FENETRE, SDL_WINDOW_RESIZABLE);
    renderer = SDL_CreateRenderer(win, -1, SDL_RENDERER_PRESENTVSYNC);
    joueurPrincipal.r = 255; joueurPrincipal.g = 255;  joueurPrincipal.b = 255;
    joueurPrincipal.corps.w = 20.0;
    joueurPrincipal.corps.h = 20.0;
    joueurPrincipal.vboost = 1.5;

    joueurPrincipal.updateVitesse();
    plateau.generationJoueurs(ReglageJeu::NB_PLAYER);
    pause = false;
}

Jeu::~Jeu(){

    TTF_Quit();
    SDL_DestroyWindow(win);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
}

bool Jeu::tick()
{
    SDL_SetRenderDrawColor(renderer, 230, 230, 230, 255);
    SDL_RenderClear(renderer);
    SDL_Event e;
    plateau.fenetreXorigin = joueurPrincipal.corps.x - (ReglageJeu::LARGEUR_FENETRE/2);
    plateau.fenetreYorigin = joueurPrincipal.corps.y - (ReglageJeu::HAUTEUR_FENETRE/2);
    for (int i = 0 ; i <=ReglageJeu::HAUTEUR_MONDE_JEU; i++)
    {
      if (i >= plateau.fenetreYorigin &&
          i <= plateau.fenetreYorigin+ReglageJeu::HAUTEUR_FENETRE && i%50==0){
          SDL_SetRenderDrawColor(renderer, 90, 90, 90, 255);
          
          SDL_RenderDrawLine(renderer, 0-plateau.fenetreXorigin, i-plateau.fenetreYorigin,
                             ReglageJeu::LARGEUR_MONDE_JEU-plateau.fenetreXorigin, i-plateau.fenetreYorigin);
      }
    }
    for (int i = 0 ; i <= ReglageJeu::LARGEUR_MONDE_JEU; i++)
    {
      if (i >= plateau.fenetreXorigin &&
          i <= plateau.fenetreXorigin+ReglageJeu::LARGEUR_FENETRE && i%50==0){
          SDL_SetRenderDrawColor(renderer, 90, 90, 90, 255);
          SDL_RenderDrawLine(renderer, i-plateau.fenetreXorigin,-plateau.fenetreYorigin,
                             i-plateau.fenetreXorigin, ReglageJeu::HAUTEUR_MONDE_JEU-plateau.fenetreYorigin);
      }
    }
    while ( SDL_PollEvent(&e))
    {
        switch (e.type)
        {
            case SDL_MOUSEMOTION:
            {
                double px = joueurPrincipal.corps.x, py = joueurPrincipal.corps.y;
                plateau.convertToFenetreCoords(px, py);
                double dx = e.motion.x - px;
                double dy = e.motion.y - py;
                joueurPrincipal.changeDirectionVitesse(dx, dy);
            }
                break;

            // Gestion du clic sur les boutons
            case SDL_MOUSEBUTTONDOWN:
            {
                if (pause)
                {
                    SDL_Point mouse;
                    SDL_GetMouseState(&mouse.x, &mouse.y);
                    int bg_y = ReglageJeu::HAUTEUR_FENETRE / 2 - ReglageJeu::HAUTEUR_MENU / 2;
                    int btn_x = ReglageJeu::LARGEUR_FENETRE / 2 - ReglageJeu::LARGEUR_BOUTON / 2;
                    
                    /* Restart */
                    int btn1_y = bg_y + ReglageJeu::HAUTEUR_MENU / 3;
                    SDL_Rect btn1 = { btn_x - 50, btn1_y,ReglageJeu::LARGEUR_BOUTON + 100,ReglageJeu::HAUTEUR_BOUTON };
                    if (SDL_PointInRect(&mouse, &btn1) == SDL_TRUE)
                        restart();
                    
                    /* Leave */
                    int btn2_y = bg_y + ReglageJeu::ESPACE_BOUTON + 2 * ReglageJeu::HAUTEUR_MENU / 3;
                    SDL_Rect btn2 = { btn_x - 50, btn2_y,ReglageJeu::LARGEUR_BOUTON + 100,ReglageJeu::HAUTEUR_BOUTON };
                    if (SDL_PointInRect(&mouse, &btn2) == SDL_TRUE)
                        return false;
                }

            }
                break;

            case SDL_KEYDOWN:
                {
                    switch (e.key.keysym.sym)
                    {
                        case SDLK_ESCAPE:
                            pause = !pause;
                            break;
                    }
                }
                break;

            case SDL_QUIT:
                return false;
        }
        
    }
    joueurPrincipal.deplace(pause);
    plateau.afficheJoueur(joueurPrincipal,renderer);
    
    for (auto &i: plateau.joueurs){
        if (i.estVisible){
            int jet = rand()%100;
            if (ReglageJeu::PROBABILITE > jet)
            {
                double dx = 1;
                double dy = 1;
                //Negate x
                if (rand() % 2 == 0)
                    dx = -1;
                //Negate y
                if (rand() % 2 == 0)
                    dy = -1;
                i.changeDirectionVitesse(dx, dy);
            }
            i.deplace(pause);
        }

        /*
            Code de modification du déplacement
        */
        
        if (i.collision(joueurPrincipal) && i.estVisible && joueurPrincipal.estVisible){
            if (i.peutManger(joueurPrincipal)){
                i.mange(joueurPrincipal);
            }else{
                joueurPrincipal.mange(i);
            }
        }
        if(plateau.estJoueurDansFenetre(i)){
            plateau.afficheJoueur(i,renderer);
        }
        if ((rand()%10000) == 0) {
            if(!pause)
                plateau.ajoutStaticElem(1);
        }
        
    }

    // Gestion de l'etat de la partie, et affichage du menu en conséquence
    switch (checkVictory())
    {
    case 0:
        if(pause)
            buildMenu(renderer, "MENU");
        break;

    case 1:
        pause = true;
        buildMenu(renderer, "Vous avez gagné");
        break;
    case -1:
        pause = true;
        buildMenu(renderer, "Vous avez perdu");
        break;
    default:
        break;
    }

    SDL_RenderPresent(renderer);
    return true;
}

void Jeu::buildMenu(SDL_Renderer* renderer, const char* state) {

            
    SDL_Color Black = { 0,0,0,255 };
    TTF_Font* font = TTF_OpenFont("Arial.ttf", 64);
    std::cout << SDL_GetError();
    /* Background */
    int bg_x, bg_y;
    bg_x = ReglageJeu::LARGEUR_FENETRE/2 - ReglageJeu::LARGEUR_MENU/ 2;
    bg_y = ReglageJeu::HAUTEUR_FENETRE / 2 - ReglageJeu::HAUTEUR_MENU / 2;
    SDL_Rect background = { bg_x, bg_y,ReglageJeu::LARGEUR_MENU,ReglageJeu::HAUTEUR_MENU };
    

    int btn_x = ReglageJeu::LARGEUR_FENETRE / 2 - ReglageJeu::LARGEUR_BOUTON / 2;
    int largeur_titre, xpos;
    if (state == "MENU") {
        largeur_titre = 150;
        xpos = btn_x;
    }

    else {
        largeur_titre = 300;
        xpos = btn_x - 80;
    }

    /* Title */
    int title_y = bg_y;
    SDL_Surface* surfaceMessageTitle = TTF_RenderText_Solid(font, state, Black);
    SDL_Texture* MessageTitle = SDL_CreateTextureFromSurface(renderer, surfaceMessageTitle);
    SDL_Rect Title = { xpos, title_y+20,largeur_titre,ReglageJeu::HAUTEUR_BOUTON +20 };

    /* Restart Button */
    int btn1_y = bg_y + ReglageJeu::HAUTEUR_MENU / 3;
    SDL_Surface* surfaceMessageButton1 = TTF_RenderText_Solid(font,"Rejouer",Black);
    SDL_Texture* MessageButton1 = SDL_CreateTextureFromSurface(renderer, surfaceMessageButton1);
    SDL_Rect Button1 = { btn_x, btn1_y,ReglageJeu::LARGEUR_BOUTON,ReglageJeu::HAUTEUR_BOUTON };
    SDL_Rect Button1Back = { btn_x - 50, btn1_y,ReglageJeu::LARGEUR_BOUTON + 100,ReglageJeu::HAUTEUR_BOUTON };

    /* Leave Button */
    int btn2_y = bg_y + ReglageJeu::ESPACE_BOUTON + 2*ReglageJeu::HAUTEUR_MENU / 3;
    SDL_Surface* surfaceMessageButton2 = TTF_RenderText_Solid(font, "Quitter", Black);
    SDL_Texture* MessageButton2 = SDL_CreateTextureFromSurface(renderer, surfaceMessageButton2);
    SDL_Rect Button2 = { btn_x, btn2_y,ReglageJeu::LARGEUR_BOUTON,ReglageJeu::HAUTEUR_BOUTON };
    SDL_Rect Button2Back = { btn_x-50, btn2_y,ReglageJeu::LARGEUR_BOUTON+100,ReglageJeu::HAUTEUR_BOUTON };
    
    /* Render Menu */
    SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
    SDL_RenderFillRect(renderer, &background);
    SDL_SetRenderDrawColor(renderer, 120, 120, 120, 255);
    SDL_RenderFillRect(renderer, &Button1Back);
    SDL_RenderFillRect(renderer, &Button2Back);
    SDL_RenderCopy(renderer, MessageButton1, NULL, &Button1);
    SDL_RenderCopy(renderer, MessageButton2, NULL, &Button2);
    SDL_RenderCopy(renderer, MessageTitle, NULL, &Title);

    TTF_CloseFont(font);
    SDL_DestroyTexture(MessageButton1);
    SDL_FreeSurface(surfaceMessageButton1);
    SDL_DestroyTexture(MessageButton2);
    SDL_FreeSurface(surfaceMessageButton2);
    SDL_DestroyTexture(MessageTitle);
    SDL_FreeSurface(surfaceMessageTitle);
}

void Jeu::restart() {
    joueurPrincipal.corps.x = ReglageJeu::LARGEUR_FENETRE / 2.0;
    joueurPrincipal.corps.y = ReglageJeu::HAUTEUR_FENETRE / 2.0;
    joueurPrincipal.taille = 30.0;
    joueurPrincipal.corps.w = 20.0;
    joueurPrincipal.corps.h = 20.0;
    joueurPrincipal.vboost = 1.5;
    joueurPrincipal.updateVitesse();
    joueurPrincipal.estVisible = true;
        plateau.generationJoueurs(ReglageJeu::NB_PLAYER);
    pause = false;
}

/* 
*   Valeur de retour
    0 = Partie en cours
    -1 = Partie perdue
    1 = Partie gagnee
*/
int Jeu::checkVictory() {
    if (!joueurPrincipal.estVisible)
        return -1;
    for (auto& i : plateau.joueurs) {
        if (i.estVisible && !i.estStatique) {
            return 0;
        }
    }
    return 1;
}