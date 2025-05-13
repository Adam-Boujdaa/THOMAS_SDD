#ifndef LOGIN_H
#define LOGIN_H

#include "voyageur.h"

// Fonction pour hacher un mot de passe avec SHA256
void hash_mot_de_passe(const char* mot_de_passe, char* mot_de_passe_hash);

// Fonction de connexion
Voyageur connexion(int identifiant, const char* mot_de_passe, Voyageur* liste, int taille);

#endif // LOGIN_H