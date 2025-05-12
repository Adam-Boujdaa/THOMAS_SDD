#ifndef VOYAGEUR_H
#define VOYAGEUR_H

#include "trajet.h"
#include "place.h"

#define MAX_RESERVATIONS 50
#define TAILLE_NOM 100
#define MAX_VOYAGEURS 100 // Nombre maximum de voyageurs

// Structure représentant un voyageur
typedef struct s_voyageur* Voyageur;

// Fonction pour créer un voyageur
Voyageur voyageur_init(const char* nom, int identifiant);

// Fonction pour effacer un voyageur
void voyageur_effacer(Voyageur* voyageur);

// Fonction pour obtenir l'identifiant d'un voyageur
int voyageur_get_identifiant(Voyageur v);

// Fonction pour obtenir le nom d'un voyageur
const char* voyageur_get_nom(Voyageur v);

// Fonction pour rechercher un voyageur par identifiant
Voyageur voyageur_rechercher_par_id(int identifiant, Voyageur* liste, int taille);

// Fonction pour réserver un trajet
int voyageur_reserver_trajet(Voyageur voyageur, Trajet trajet);

// Fonction pour modifier un trajet réservé
int voyageur_modifier_trajet(Voyageur voyageur, Trajet ancien, Trajet nouveau);

// Fonction pour afficher les réservations d'un voyageur
void voyageur_afficher_reservations(Voyageur voyageur);

// Fonction pour afficher le menu principal
void menu_principal(Voyageur* liste_voyageurs, int* nb_voyageurs, Trajet* liste_trajets, int nb_trajets);

#endif // VOYAGEUR_H
