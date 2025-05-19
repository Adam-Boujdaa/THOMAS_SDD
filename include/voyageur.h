#ifndef VOYAGEUR_H
#define VOYAGEUR_H

#include "trajet.h" // Ensure MAX_TRAJETS is defined

#define MAX_VOYAGEURS 100
#define MAX_USERNAME_LENGTH 50
#define MAX_PASSWORD_LENGTH 256

// IMPORTANT : Le champ 'identifiant' de Voyageur doit correspondre exactement au 'username' de User (auth.h)
// Cela permet de relier l'utilisateur authentifié à son profil de voyageur pour les réservations.
// Lorsqu'un utilisateur se connecte, on recherche le Voyageur avec le même identifiant pour gérer ses trajets et réservations.

typedef struct {
    char nom[50];
    char identifiant[MAX_USERNAME_LENGTH];
    char password_hash[MAX_PASSWORD_LENGTH]; // Mot de passe haché pour l'authentification
    Trajet trajets[MAX_TRAJETS];
    int nombre_trajets;
    char reservations[MAX_TRAJETS][256]; // noms des trajets réservés (taille augmentée)
    int nombre_reservations;
} Voyageur;

typedef struct s_voyageurDB {
    Voyageur voyageurs[MAX_VOYAGEURS];
    int nombre_voyageurs;
} VoyageurDB;

void ajouter_voyageur(Voyageur *voyageur, const char *nom, const char *identifiant);
void reserver_trajet(Voyageur *voyageur, const Trajet *trajet);
void afficher_voyageur(const Voyageur *voyageur);

#endif