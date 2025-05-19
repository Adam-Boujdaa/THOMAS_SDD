#include "login.h"
#include <string.h>
#include <openssl/sha.h>

void hash_mot_de_passe(const char* mot_de_passe, char* mot_de_passe_hash) {
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256((unsigned char*)mot_de_passe, strlen(mot_de_passe), hash);
    for (int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
        sprintf(mot_de_passe_hash + (i * 2), "%02x", hash[i]);
    }
    mot_de_passe_hash[SHA256_DIGEST_LENGTH * 2] = '\0';
}

int check_hash(const char* mot_de_passe, const char* mot_de_passe_hash) {
    char hash[SHA256_DIGEST_LENGTH * 2 + 1];
    hash_mot_de_passe(mot_de_passe, hash);
    return strcmp(hash, mot_de_passe_hash) == 0;
}

Voyageur connexion(int identifiant, const char* mot_de_passe, Voyageur* liste, int taille) {
    for (int i = 0; i < taille; i++) {
        if (liste[i]->identifiant == identifiant && check_hash(mot_de_passe, liste[i]->mot_de_passe_hash)) {
            return liste[i];
        }
    }
    return NULL;
}