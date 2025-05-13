#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "voyageur.h"
#include "trajet.h"
#include "login.h"

#define TAILLE_NOM 100
#define MAX_RESERVATIONS 10

// Define the global VoyageurDB instance
VoyageurDB voyageurDB = { .nombre_voyageurs = 0 };

// Initialiser un voyageur avec un nom, un identifiant et un mot de passe
Voyageur voyageur_init(const char* nom, int identifiant, const char* mot_de_passe) {
    Voyageur v = malloc(sizeof(struct s_voyageur));
    if (!v) return NULL;
    strncpy(v->nom, nom, TAILLE_NOM - 1);
    v->nom[TAILLE_NOM - 1] = '\0';
    v->identifiant = identifiant;
    v->nombre_reservations = 0;

    return v;
}

// Supprimer un voyageur
void voyageur_effacer(Voyageur* voyageur) {
    if (*voyageur) {
        free(*voyageur);
        *voyageur = NULL;
    }
}

// Récupérer l'identifiant d'un voyageur
int voyageur_get_identifiant(Voyageur v) {
    return v->identifiant;
}

// Récupérer le nom d'un voyageur
const char* voyageur_get_nom(Voyageur v) {
    return v->nom;
}

// Rechercher un voyageur par identifiant
Voyageur voyageur_rechercher_par_id(int identifiant, Voyageur* liste, int taille) {
    for (int i = 0; i < taille; i++) {
        if (liste[i] && liste[i]->identifiant == identifiant) {
            return liste[i];
        }
    }
    return NULL;
}

// Ajouter une réservation pour un voyageur
int voyageur_reserver_trajet(Voyageur voyageur, Trajet trajet) {
    if (!voyageur || !trajet || voyageur->nombre_reservations >= MAX_RESERVATIONS) return 0;
    if (trajet_places_libres(trajet) <= 0) return 0;  // On vérifie qu'il y a des places libres

    // Réserver une place
    voyageur->trajets_reserves[voyageur->nombre_reservations++] = trajet;
    return 1;
}

// Modifier une réservation de trajet
int voyageur_modifier_trajet(Voyageur voyageur, Trajet ancien, Trajet nouveau) {
    for (int i = 0; i < voyageur->nombre_reservations; i++) {
        if (trajet_get_id(voyageur->trajets_reserves[i]) == trajet_get_id(ancien)) {
            if (trajet_places_libres(nouveau) > 0) {
                voyageur->trajets_reserves[i] = nouveau;
                return 1;
            }
        }
    }
    return 0;
}

// Afficher les réservations d'un voyageur
void voyageur_afficher_reservations(Voyageur voyageur) {
    printf("\n--- Réservations de %s ---\n", voyageur->nom);
    if (voyageur->nombre_reservations == 0) {
        printf("Aucune réservation.\n");
    } else {
        for (int i = 0; i < voyageur->nombre_reservations; i++) {
            printf("Réservation %d : ID %d\n", i + 1, trajet_get_id(voyageur->trajets_reserves[i]));
        }
    }
}

Voyageur voyageurs[MAX_VOYAGEURS];
int nombre_voyageurs = 0;