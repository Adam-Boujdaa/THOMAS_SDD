#include "place.h"
#include <string.h>

// Initialise une place avec un numéro de siège et un wagon donnés, et définit est_reservee à 0
Place place_creer(int num_siege, char* voiture) {
    Place place;
    place.num_siege = num_siege;

    // Allouer de la mémoire pour le nom de la voiture et copier la valeur
    place.voiture = (char*)malloc(strlen(voiture) + 1);
    if (place.voiture != NULL) {
        strcpy(place.voiture, voiture);
    }

    place.est_reservee = 0; // Par défaut, la place n'est pas réservée
    return place;
}

// Réserve la place
void place_reserver(Place* place) {
    if (place != NULL) {
        place->est_reservee = 1; // Marquer la place comme réservée
    }
}

// Annule la réservation de la place
void place_annuler_reservation(Place* place) {
    if (place != NULL) {
        place->est_reservee = 0; // Marquer la place comme non réservée
    }
}

// Vérifie si la place est réservée (1 pour réservé, 0 pour non réservé)
int place_est_reservee(const Place* place) {
    if (place != NULL) {
        return place->est_reservee;
    }
    return 0; // Retourne 0 si la place est NULL
}-