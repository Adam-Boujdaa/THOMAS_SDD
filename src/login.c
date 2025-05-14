#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#include "login.h"
#include <stdio.h>
#include <string.h>
#include <openssl/sha.h>
#include "voyageur.h"
#include "cJSON.h"
#include <stdlib.h>

// Function to read the administrator password from a config.json file
char* read_admin_password(const char* config_path, enum user_type type) {
    FILE* file = fopen(config_path, "r");
    if (!file) {
        perror("Failed to open config file");
        return NULL;
    }

    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    char* file_content = (char*)malloc(file_size + 1);
    if (!file_content) {
        perror("Failed to allocate memory for config file content");
        fclose(file);
        return NULL;
    }

    fread(file_content, 1, file_size, file);
    file_content[file_size] = '\0';
    fclose(file);

    cJSON* json = cJSON_Parse(file_content);
    free(file_content);

    if (!json) {
        fprintf(stderr, "Failed to parse config file: %s\n", cJSON_GetErrorPtr());
        return NULL;
    }

    const char* key = (type == ADMIN) ? "admin_mdp" : "controlleur_mdp";
    cJSON* password_item = cJSON_GetObjectItemCaseSensitive(json, key);

    if (!cJSON_IsString(password_item) || (password_item->valuestring == NULL)) {
        fprintf(stderr, "Password not found or invalid in config file\n");
        cJSON_Delete(json);
        return NULL;
    }

    char* password = strdup(password_item->valuestring);
    cJSON_Delete(json);

    return password;
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