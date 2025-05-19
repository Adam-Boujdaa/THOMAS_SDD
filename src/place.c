#include "place.h"
#include <stdio.h>

// Fonction pour initialiser les places disponibles
void initialiser_places(Place places[MAX_PLACES]) {
    for (int i = 0; i < MAX_PLACES; i++) {
        places[i].numero = i + 1;
        places[i].est_occupee = false;
        places[i].voyageur = NULL;
    }
}

// Fonction pour réserver une place pour un voyageur
bool reserver_place(Place places[MAX_PLACES], Voyageur *voyageur) {
    for (int i = 0; i < MAX_PLACES; i++) {
        if (!places[i].est_occupee) {
            places[i].est_occupee = true;
            places[i].voyageur = voyageur;
            return true;
        }
    }
    return false;
}

// Fonction pour libérer une place donnée par son numéro
void liberer_place(Place places[MAX_PLACES], int numero) {
    if (numero > 0 && numero <= MAX_PLACES) {
        places[numero - 1].est_occupee = false;
        places[numero - 1].voyageur = NULL;
    }
}

// Fonction pour afficher l'état de toutes les places
void afficher_places(const Place places[MAX_PLACES]) {
    for (int i = 0; i < MAX_PLACES; i++) {
        printf("Place %d: %s\n", places[i].numero, places[i].est_occupee ? "Occupée" : "Libre");
    }
}