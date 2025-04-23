#ifndef __VOYAGEUR_H__
#define __VOYAGEUR_H__

#include "trajet.h"
#include "place.h"

typedef struct s_voyageur * Voyageur;

// Création et destruction
Voyageur voyageur_init(const char* nom, int id);
void voyageur_effacer(Voyageur* voyageur);

// Recherche d’un voyageur par identifiant
Voyageur voyageur_rechercher_par_id(int id, Voyageur* liste, int taille);

// Réservation et modification de trajet
int voyageur_reserver_trajet(Voyageur voyageur, Trajet trajet, Place* places, int nb_places);
// Retourne 1 si réservation réussie, 0 sinon (places indisponibles, etc.)

int voyageur_modifier_trajet(Voyageur voyageur, Trajet ancien, Trajet nouveau, Place* places, int nb_places);
// Retourne 1 si modification réussie, 0 sinon

// Navigation dans le menu voyageur (recherche par gare de départ/arrivée)
void menu_voyageur(Voyageur* liste_voyageurs, int nb_voyageurs, Trajet* liste_trajets, int nb_trajets, Place* places, int nb_places);

// Vérification de la cohérence réservation/places disponibles
int trajet_places_disponibles(Trajet trajet, Place* places, int nb_places);
// Retourne le nombre de places libres pour un trajet

#endif //
