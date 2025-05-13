#include "place.h"
#include <string.h>
#include "voyageur.h" // Include Voyageur for references

struct s_place{
    int num_siege; // Seat number
    int voiture; // Car number
    int est_reservee; // Indicates if the place is reserved (1 for reserved, 0 for not reserved)
    Voyageur* voyageurs; // Array of Voyageur references
    int nombre_voyageurs; // Number of Voyageurs occupying the place
};

// Initialise une place avec un numéro de siège et un wagon donnés, et définit est_reservee à 0
Place place_creer(int num_siege, int voiture) {
    Place place = malloc(sizeof(struct s_place));
    if (place == NULL) {
        fprintf(stderr, "Erreur : allocation mémoire pour la place.\n");
    }
    place->num_siege = num_siege;

    // Allouer de la mémoire pour le nom de la voiture et copier la valeur
    place->voiture = voiture;

    place->est_reservee = 0; // Par défaut, la place n'est pas réservée
    return place;
}

// Réserve la place
void place_reserver(Place place) {
    if (place != NULL) {
        place->est_reservee = 1; // Marquer la place comme réservée
    }
}

// Annule la réservation de la place
void place_annuler_reservation(Place place) {
    if (place != NULL) {
        place->est_reservee = 0; // Marquer la place comme non réservée
    }
}

// Vérifie si la place est réservée (1 pour réservé, 0 pour non réservé)
int place_est_reservee(const Place place) {
    if (place != NULL) {
        return place->est_reservee;
    }
    return 0; // Retourne 0 si la place est NULL
}

// Add a Voyageur to a Place
void place_ajouter_voyageur(Place place, Voyageur voyageur) {
    if (place == NULL || voyageur == NULL) return;

    place->voyageurs = realloc(place->voyageurs, (place->nombre_voyageurs + 1) * sizeof(Voyageur));
    if (place->voyageurs == NULL) {
        fprintf(stderr, "Erreur : allocation mémoire pour ajouter un voyageur.\n");
        exit(EXIT_FAILURE);
    }

    place->voyageurs[place->nombre_voyageurs++] = voyageur;
}

// Remove a Voyageur from a Place
void place_retirer_voyageur(Place place, Voyageur voyageur) {
    if (place == NULL || voyageur == NULL) return;

    for (int i = 0; i < place->nombre_voyageurs; i++) {
        if (place->voyageurs[i] == voyageur) {
            for (int j = i; j < place->nombre_voyageurs - 1; j++) {
                place->voyageurs[j] = place->voyageurs[j + 1];
            }
            place->nombre_voyageurs--;
            place->voyageurs = realloc(place->voyageurs, place->nombre_voyageurs * sizeof(Voyageur));
            return;
        }
    }
}

// List all Voyageurs occupying a Place
void place_lister_voyageurs(const Place place) {
    if (place == NULL) return;

    printf("Voyageurs occupant la place %d dans la voiture %d :\n", place->num_siege, place->voiture);
    for (int i = 0; i < place->nombre_voyageurs; i++) {
        printf("- %s\n", voyageur_get_nom(place->voyageurs[i]));
    }
}