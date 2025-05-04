#ifndef VOYAGEUR_H
#define VOYAGEUR_H

#define MAX_NOM 100
#define MAX_TRAJETS 10

typedef struct s_voyageur* Voyageur;


Voyageur creer_voyageur(const char* nom, int id);
void reserver_trajet(Voyageur voyageur, int id_trajet);
void modifier_trajet(Voyageur voyageur, int ancien_trajet, int nouveau_trajet);
int trajet_existe(Voyageur voyageur, int id_trajet);
void afficher_voyageur(Voyageur voyageur);
void liberer_voyageur(Voyageur voyageur);

#endif
