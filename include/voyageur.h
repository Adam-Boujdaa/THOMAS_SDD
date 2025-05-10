#ifndef VOYAGEUR_H
#define VOYAGEUR_H

#define MAX_RESERVATIONS 50
#define TAILLE_NOM 100

#include "trajet.h"
#include "place.h"

typedef struct s_voyageur* Voyageur;

Voyageur voyageur_init(const char* nom, int identifiant);
void voyageur_effacer(Voyageur* voyageur);
Voyageur voyageur_rechercher_par_id(int identifiant, Voyageur* liste, int taille);
int voyageur_reserver_trajet(Voyageur voyageur, Trajet trajet, Place* places, int nb_places);
int voyageur_modifier_trajet(Voyageur voyageur, Trajet ancien, Trajet nouveau, Place* places, int nb_places);
int trajet_places_disponibles(Trajet trajet, Place* places, int nb_places);
void menu_voyageur(Voyageur* liste_voyageurs, int nb_voyageurs, Trajet* liste_trajets, int nb_trajets, Place* places, int nb_places);

#endif
