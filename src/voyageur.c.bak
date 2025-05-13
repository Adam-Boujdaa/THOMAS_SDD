#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "voyageur.h"
#include "trajet.h"
#include <openssl/sha.h>  // Bibliothèque pour SHA256

#define TAILLE_NOM 100
#define TAILLE_MOT_DE_PASSE 64
#define MAX_RESERVATIONS 10


// Structure représentant un voyageur
struct s_voyageur {
    char nom[TAILLE_NOM];
    int identifiant;
    char mot_de_passe_hash[TAILLE_MOT_DE_PASSE];  // Mot de passe haché
    Trajet trajets_reserves[MAX_RESERVATIONS];     // Tableau des trajets réservés
    int nombre_reservations;
};

// Fonction pour hacher un mot de passe avec SHA256
void hacher_mot_de_passe(const char* mot_de_passe, char* mot_de_passe_hash) {
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256_CTX sha256_ctx;

    SHA256_Init(&sha256_ctx);
    SHA256_Update(&sha256_ctx, mot_de_passe, strlen(mot_de_passe));
    SHA256_Final(hash, &sha256_ctx);

    // Convertir le hash en chaîne hexadécimale
    for (int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
        sprintf(mot_de_passe_hash + (i * 2), "%02x", hash[i]);
    }
}

// Initialiser un voyageur avec un nom, un identifiant et un mot de passe
Voyageur voyageur_init(const char* nom, int identifiant, const char* mot_de_passe) {
    Voyageur v = malloc(sizeof(struct s_voyageur));
    if (!v) return NULL;
    strncpy(v->nom, nom, TAILLE_NOM - 1);
    v->nom[TAILLE_NOM - 1] = '\0';
    v->identifiant = identifiant;
    v->nombre_reservations = 0;

    // Hacher et stocker le mot de passe
    hacher_mot_de_passe(mot_de_passe, v->mot_de_passe_hash);

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

// Vérifier le mot de passe lors de la connexion
int voyageur_verifier_mot_de_passe(Voyageur v, const char* mot_de_passe) {
    char mot_de_passe_hash[TAILLE_MOT_DE_PASSE];
    hacher_mot_de_passe(mot_de_passe, mot_de_passe_hash);

    return (strncmp(v->mot_de_passe_hash, mot_de_passe_hash, TAILLE_MOT_DE_PASSE) == 0);
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


int creer_compte(const char* nom, int identifiant, const char* mot_de_passe) {
    // Vérifier si le nombre maximum de voyageurs a été atteint
    if (nombre_voyageurs >= MAX_VOYAGEURS) {
        printf("Le nombre maximum de comptes a été atteint.\n");
        return 0;
    }

    // Vérifier si l'identifiant est déjà utilisé
    for (int i = 0; i < nombre_voyageurs; i++) {
        if (voyageurs[i]->identifiant == identifiant) {
            printf("L'identifiant est déjà utilisé.\n");
            return 0;
        }
    }

    // Hacher le mot de passe
    char mot_de_passe_hache[TAILLE_HASH];
    hacher_mot_de_passe(mot_de_passe, mot_de_passe_hache);

    // Créer un nouveau voyageur avec le mot de passe haché
    Voyageur v = voyageur_init(nom, identifiant, mot_de_passe_hache);
    if (!v) {
        printf("Erreur lors de la création du compte.\n");
        return 0;
    }

    // Ajouter le voyageur à la liste
    voyageurs[nombre_voyageurs++] = v;
    printf("Compte créé avec succès.\n");
    return 1;
}


// Fonction de connexion
Voyageur connexion(int identifiant, const char* mot_de_passe) {
    // Recherche du voyageur par identifiant
    for (int i = 0; i < nombre_voyageurs; i++) {
        if (voyageurs[i]->identifiant == identifiant) {
            // Vérifier le mot de passe
            if (verifier_mot_de_passe(voyageurs[i]->mot_de_passe_hache, mot_de_passe)) {
                printf("Connexion réussie.\n");
                return voyageurs[i];
            } else {
                printf("Mot de passe incorrect.\n");
                return NULL;
            }
        }
    }
    printf("Identifiant non trouvé.\n");
    return NULL;
}


int supprimer_compte(int identifiant) {
    for (int i = 0; i < nombre_voyageurs; i++) {
        if (voyageurs[i] && voyageurs[i]->identifiant == identifiant) {
            // Libérer la mémoire
            voyageur_effacer(&voyageurs[i]);

            // Décaler les éléments suivants
            for (int j = i; j < nombre_voyageurs - 1; j++) {
                voyageurs[j] = voyageurs[j + 1];
            }

            voyageurs[nombre_voyageurs - 1] = NULL;
            nombre_voyageurs--;

            printf("Compte supprimé avec succès.\n");
            return 1;
        }
    }

    printf("Identifiant introuvable. Suppression impossible.\n");
    return 0;
}
