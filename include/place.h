#ifndef PLACE_H
#define PLACE_H

#include "trajet.h"
#include "voyageur.h"

#define MAX_PLACES 10

typedef struct {
    int numero;
    bool est_occupee;
    Voyageur *voyageur;
} Place;

void initialiser_places(Place places[MAX_PLACES]);
bool reserver_place(Place places[MAX_PLACES], Voyageur *voyageur);
void liberer_place(Place places[MAX_PLACES], int numero);
void afficher_places(const Place places[MAX_PLACES]);

#endif