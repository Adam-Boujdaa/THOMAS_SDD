#ifndef AUTH_H
#define AUTH_H

#include <stdbool.h>
#include "voyageur.h"

// Authentification et gestion des comptes directement sur Voyageur
void init_voyageurs();
bool authenticate_voyageur(const char *identifiant, const char *password, VoyageurDB *db);
bool change_password_voyageur(const char *identifiant, const char *new_password, VoyageurDB *db);
void save_voyageurs_to_file(const char *filename, VoyageurDB *db);
void load_voyageurs_from_file(const char *filename, VoyageurDB *db);
bool creer_voyageur(const char *identifiant, const char *password, const char *nom, VoyageurDB *db);

#endif