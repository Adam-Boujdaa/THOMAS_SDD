#include <stdlib.h>

// Structure représentant une place
typedef struct {
    int num_siege; // Numéro du siège
    char* voiture;    // Numéro de la voiture
    int est_reservee; // Indique si la place est réservée (1 pour réservé, 0 pour non réservé)
} Place;

// Initialise une place avec un numéro de siège et un wagon donnés, et définit isReserved à 0
Place place_creer(int num_siege, char* voiture);

// Réserve la place
void place_reserver(Place* place);

// Annule la réservation de la place
void place_annuler_reservation(Place* place);

// Vérifie si la place est réservée (1 pour réservé, 0 pour non réservé)
int place_est_reservee(const Place* place);