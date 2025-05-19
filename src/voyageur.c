#include "voyageur.h"
#include "trajet.h"
#include <stdio.h>
#include <string.h>

// Fonction pour ajouter un voyageur avec son nom et identifiant
void ajouter_voyageur(Voyageur *voyageur, const char *nom, const char *identifiant) {
    strncpy(voyageur->nom, nom, sizeof(voyageur->nom));
    strncpy(voyageur->identifiant, identifiant, sizeof(voyageur->identifiant));
    voyageur->nombre_trajets = 0;
}

// Fonction pour réserver un trajet pour un voyageur
void reserver_trajet(Voyageur *voyageur, const Trajet *trajet) {
    if (voyageur->nombre_trajets < MAX_TRAJETS) {
        voyageur->trajets[voyageur->nombre_trajets] = *trajet;
        voyageur->nombre_trajets++;
    }
}

// Fonction pour afficher les informations d'un voyageur
void afficher_voyageur(const Voyageur *voyageur) {
    printf("Voyageur: %s, ID: %s\n", voyageur->nom, voyageur->identifiant);
    for (int i = 0; i < voyageur->nombre_trajets; i++) {
        trajet_afficher(&voyageur->trajets[i]);
    }
}