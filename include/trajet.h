#ifndef TRAJET_H
#define TRAJET_H

#include <stdbool.h>

#define MAX_GARES 10
#define MAX_TRAJETS 100

typedef struct {
    char nom[50];
    float temps; // Temps en heures depuis le départ
} Gare;

typedef struct {
    char nom[50];
    Gare gares[MAX_GARES];
    int nombre_gares;
    int capacite; // nombre total de places
    int places_libres; // nombre de places libres
} Trajet;

typedef struct s_trajetDB {
    Trajet trajets[MAX_TRAJETS];
    int nombre_trajets;
} TrajetDB;

Trajet trajet_init(int id); // Removed the unused `max_gares` parameter
void trajet_ajouter_gare(Trajet *trajet, const char *nom, float temps);
void trajet_afficher(const Trajet *trajet);
Trajet trajet_fusionner(const Trajet *trajet1, const Trajet *trajet2);
void trajet_prolonger(Trajet *trajet, const char *nom, float temps);
void trajet_raccourcir(Trajet *trajet, const char *nom);
void trajet_sauvegarder(const Trajet *trajet, const char *filename);
Trajet trajet_charger(const char *filename);
void trajet_effacer(Trajet *trajet);
Trajet trajet_rechercher(const char *gare_depart, const char *gare_arrivee);
void modifier_trajet_interactif(); // Declare the function
void importer_trajets_depuis_json(const char *filename);
void exporter_trajets_txt();

#endif