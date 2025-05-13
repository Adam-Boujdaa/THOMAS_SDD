#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#include "login.h"
#include <stdio.h>
#include <string.h>
#include <openssl/sha.h>
#include "voyageur.h" // Include to define struct s_voyageur

// Fonction pour hacher un mot de passe avec SHA256
void hash_mot_de_passe(const char* mot_de_passe, char* mot_de_passe_hash) {
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

// Fonction de connexion
Voyageur connexion(int identifiant, const char* mot_de_passe, Voyageur* liste, int taille) {
    for (int i = 0; i < taille; i++) {
        if (liste[i] && liste[i]->identifiant == identifiant) {
            char mot_de_passe_hash[TAILLE_MOT_DE_PASSE];
            hash_mot_de_passe(mot_de_passe, mot_de_passe_hash);

            if (strncmp(liste[i]->mot_de_passe_hash, mot_de_passe_hash, TAILLE_MOT_DE_PASSE) == 0) {
                printf("Connexion réussie.\n");
                return liste[i];
            } else {
                printf("Mot de passe incorrect.\n");
                return NULL;
            }
        }
    }
    printf("Identifiant non trouvé.\n");
    return NULL;
}