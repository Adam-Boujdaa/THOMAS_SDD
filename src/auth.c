#include "auth.h"
#include "voyageur.h"
#include <stdio.h>
#include <string.h>
#include <openssl/sha.h>

// Initialisation de la base de voyageurs
void init_voyageurs() {
    // Rien à faire ici, la structure est initialisée ailleurs
}

// Authentification d'un voyageur
bool authenticate_voyageur(const char *identifiant, const char *password, VoyageurDB *db) {
    char hash[SHA256_DIGEST_LENGTH];
    char hash_hex[SHA256_DIGEST_LENGTH * 2 + 1];
    SHA256((unsigned char *)password, strlen(password), (unsigned char *)hash);
    for (int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
        sprintf(hash_hex + (i * 2), "%02x", (unsigned char)hash[i]);
    }
    hash_hex[SHA256_DIGEST_LENGTH * 2] = '\0';
    for (int i = 0; i < db->nombre_voyageurs; i++) {
        if (strcmp(db->voyageurs[i].identifiant, identifiant) == 0 && strcmp(db->voyageurs[i].password_hash, hash_hex) == 0) {
            return true;
        }
    }
    return false;
}

// Changer le mot de passe d'un voyageur
bool change_password_voyageur(const char *identifiant, const char *new_password, VoyageurDB *db) {
    char hash[SHA256_DIGEST_LENGTH];
    char hash_hex[SHA256_DIGEST_LENGTH * 2 + 1];
    SHA256((unsigned char *)new_password, strlen(new_password), (unsigned char *)hash);
    for (int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
        sprintf(hash_hex + (i * 2), "%02x", (unsigned char)hash[i]);
    }
    hash_hex[SHA256_DIGEST_LENGTH * 2] = '\0';
    for (int i = 0; i < db->nombre_voyageurs; i++) {
        if (strcmp(db->voyageurs[i].identifiant, identifiant) == 0) {
            strncpy(db->voyageurs[i].password_hash, hash_hex, sizeof(db->voyageurs[i].password_hash));
            return true;
        }
    }
    return false;
}

// Sauvegarder les voyageurs dans un fichier texte
void save_voyageurs_to_file(const char *filename, VoyageurDB *db) {
    FILE *file = fopen(filename, "w");
    if (!file) {
        perror("Erreur lors de la création/écriture de voyageur.txt");
        return;
    }
    fprintf(file, "%d\n", db->nombre_voyageurs);
    for (int i = 0; i < db->nombre_voyageurs; i++) {
        fprintf(file, "%s %s %s\n", db->voyageurs[i].identifiant, db->voyageurs[i].password_hash, db->voyageurs[i].nom);
    }
    fclose(file);
}

// Charger les voyageurs depuis un fichier texte
void load_voyageurs_from_file(const char *filename, VoyageurDB *db) {
    FILE *file = fopen(filename, "r");
    db->nombre_voyageurs = 0;
    if (file) {
        int n;
        if (fscanf(file, "%d\n", &n) == 1) {
            for (int i = 0; i < n; i++) {
                char ident[100], hash[100], nom[100];
                int res = fscanf(file, "%99s %99s %99s\n", ident, hash, nom);
                if (res == 3) {
                    strncpy(db->voyageurs[db->nombre_voyageurs].identifiant, ident, sizeof(db->voyageurs[db->nombre_voyageurs].identifiant));
                    strncpy(db->voyageurs[db->nombre_voyageurs].password_hash, hash, sizeof(db->voyageurs[db->nombre_voyageurs].password_hash));
                    strncpy(db->voyageurs[db->nombre_voyageurs].nom, nom, sizeof(db->voyageurs[db->nombre_voyageurs].nom));
                    db->voyageurs[db->nombre_voyageurs].nombre_trajets = 0;
                    db->voyageurs[db->nombre_voyageurs].nombre_reservations = 0;
                    db->nombre_voyageurs++;
                }
            }
        }
        fclose(file);
    }
}

// Créer un nouveau voyageur
bool creer_voyageur(const char *identifiant, const char *password, const char *nom, VoyageurDB *db) {
    if (db->nombre_voyageurs >= MAX_VOYAGEURS) {
        return false;
    }
    // Vérifier que l'identifiant n'existe pas déjà
    for (int i = 0; i < db->nombre_voyageurs; i++) {
        if (strcmp(db->voyageurs[i].identifiant, identifiant) == 0) {
            return false;
        }
    }
    // Création du voyageur
    Voyageur *voyageur = &db->voyageurs[db->nombre_voyageurs];
    strncpy(voyageur->nom, nom, sizeof(voyageur->nom));
    strncpy(voyageur->identifiant, identifiant, sizeof(voyageur->identifiant));
    char hash[SHA256_DIGEST_LENGTH];
    char hash_hex[SHA256_DIGEST_LENGTH * 2 + 1];
    SHA256((unsigned char *)password, strlen(password), (unsigned char *)hash);
    for (int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
        sprintf(hash_hex + (i * 2), "%02x", (unsigned char)hash[i]);
    }
    hash_hex[SHA256_DIGEST_LENGTH * 2] = '\0';
    strncpy(voyageur->password_hash, hash_hex, sizeof(voyageur->password_hash));
    voyageur->nombre_trajets = 0;
    voyageur->nombre_reservations = 0;
    db->nombre_voyageurs++;
    return true;
}