#include <stdlib.h>
#include "voyageur.h" // Include Voyageur for references

#define MAX_NOM_VOITURE 20 // Longueur maximale du nom d'une voiture

typedef struct s_place* Place;

// Initialise une place avec un numéro de siège et un wagon donnés, et définit isReserved à 0
Place place_creer(int num_siege, int voiture);

// Réserve la place
void place_reserver(Place place);

// Annule la réservation de la place
void place_annuler_reservation(Place place);

// Vérifie si la place est réservée (1 pour réservé, 0 pour non réservé)
int place_est_reservee(const Place place);

// Add a Voyageur to a Place
void place_ajouter_voyageur(Place place, Voyageur voyageur);

// Remove a Voyageur from a Place
void place_retirer_voyageur(Place place, Voyageur voyageur);

// List all Voyageurs occupying a Place
void place_lister_voyageurs(const Place place);