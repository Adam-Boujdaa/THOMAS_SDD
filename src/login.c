#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#include "login.h"
#include <stdio.h>
#include <string.h>
#include <openssl/sha.h>
#include "voyageur.h"
#include <dotenv.h>

// Function to read the administrator password from a dotenv file
char* read_admin_password(const char* dotenv_path, enum user_type) {
    // Load the dotenv file
    if (dotenv_load(dotenv_path) != 0) {
        fprintf(stderr, "Failed to load dotenv file: %s\n", dotenv_path);
        return NULL;
    }

    if (user_type == ADMIN) {
        const char* password_hash = dotenv_get("MDP_ADMIN");
    } else if (user_type == CONTROLLEUR) {
        const char* password_hash = dotenv_get("MDP_CONTROLLEUR");
    }
    
    if (password_hash == NULL) {
        fprintf(stderr, "Password hash not found in dotenv file\n");
        return NULL;
    }

    // Return a copy of the password (caller is responsible for freeing it)
    return strdup(password_hash);
}

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

int check_hash(const char* mot_de_passe, const char* mot_de_passe_hash) {
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256_CTX sha256_ctx;

    SHA256_Init(&sha256_ctx);
    SHA256_Update(&sha256_ctx, mot_de_passe, strlen(mot_de_passe));
    SHA256_Final(hash, &sha256_ctx);

    // Convertir le hash en chaîne hexadécimale
    char hash_hex[SHA256_DIGEST_LENGTH * 2 + 1];
    for (int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
        sprintf(hash_hex + (i * 2), "%02x", hash[i]);
    }

    return strncmp(hash_hex, mot_de_passe_hash, TAILLE_MOT_DE_PASSE) == 0;
}

// Fonction de connexion
Voyageur* connexion(int identifiant, const char* mot_de_passe, Voyageur* liste, int taille) {
    for (int i = 0; i < taille; i++) {
        if (liste[i].identifiant == identifiant) {
            char mot_de_passe_hash[TAILLE_MOT_DE_PASSE];
            hash_mot_de_passe(mot_de_passe, mot_de_passe_hash);

            if (strncmp(liste[i].mot_de_passe_hash, mot_de_passe_hash, TAILLE_MOT_DE_PASSE) == 0) {
                printf("Connexion réussie.\n");
                return &liste[i];
            } else {
                printf("Mot de passe incorrect.\n");
                return NULL;
            }
        }
    }
    printf("Identifiant non trouvé.\n");
    return NULL;
}