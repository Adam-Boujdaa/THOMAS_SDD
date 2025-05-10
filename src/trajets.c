#define _GNU_SOURCE   //obligatoire pr bien inclure string.h pr strdup apres 
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "../include/trajet.h"

#define MAX_NOM_VILLE 85 // Longueur maximale du nom d'une ville en France

// Définition de la structure Trajet
struct s_trajet {
    char gare_depart[MAX_NOM_VILLE]; // Nom de la gare de départ
    char gare_arrivee[MAX_NOM_VILLE]; // Nom de la gare d'arrivée
    float duree; // Durée totale du trajet en heures
    char** liste_gares; // Liste des gares intermédiaires (tableau dynamique de chaînes)
    int nombre_gares; // Nombre de gares intermédiaires
    int nombre_places; // Nombre de places disponibles
    int nombre_reservations; // Nombre de réservations effectuées
    int* places; // Tableau des places 0 = non resérvées, 1 = réservées
    int id_trajet; // Identifiant unique du trajet
};


// Constructeur 
Trajet trajet_init(int id_trajet, int nombre_places) {
    Trajet trajet = malloc(sizeof(struct s_trajet));
    if (!trajet) {
        fprintf(stderr, "Erreur d'allocation mémoire pour le trajet.\n");
        exit(EXIT_FAILURE);
    }
    trajet->gare_depart[0] = '\0';
    trajet->gare_arrivee[0] = '\0';
    trajet->duree = 0.0;
    trajet->liste_gares = NULL;
    trajet->nombre_gares = 0;
    trajet->nombre_places = nombre_places;
    trajet->nombre_reservations = 0;
    trajet->places = malloc(nombre_places* sizeof(int));
    for (int i = 0; i < nombre_places; i++) {
        trajet->places[i] = 0;    // 0 = non réservé, 1 = réservé
    }
    trajet->id_trajet = id_trajet;
    return trajet;
}


// Gestion des gares dans un trajet
void trajet_ajouter_gare(Trajet trajet, char* nom_gare, float temps_depuis_depart) {
    if (!trajet || !nom_gare) return;

    trajet->liste_gares = realloc(trajet->liste_gares, (trajet->nombre_gares + 1) * sizeof(char*));
    if (!trajet->liste_gares) {
        fprintf(stderr, "Erreur : allocation mémoire pour ajouter une gare.\n");
        exit(EXIT_FAILURE);
    }

    trajet->liste_gares[trajet->nombre_gares] = strdup(nom_gare);
    if (!trajet->liste_gares[trajet->nombre_gares]) {
        fprintf(stderr, "Erreur : allocation mémoire pour le nom de la gare.\n");
        exit(EXIT_FAILURE);
    }

    trajet->nombre_gares++;
}

void trajet_supprimer_gare(Trajet trajet, char* nom_gare) {
    if (!trajet || !nom_gare) return;

    for (int i = 0; i < trajet->nombre_gares; i++) {
        if (strcmp(trajet->liste_gares[i], nom_gare) == 0) {
            free(trajet->liste_gares[i]);
            for (int j = i; j < trajet->nombre_gares - 1; j++) {
                trajet->liste_gares[j] = trajet->liste_gares[j + 1];
            }
            trajet->nombre_gares--;
            break;
        }
    }
}

void trajet_modifier_temps(Trajet trajet, char* nom_gare, float nouveau_temps) {
    // Cette fonction pourrait être utilisée pour modifier la durée associée à une gare spécifique.
    // Implémentation dépendante des besoins.
}

// Manipulation des trajets
Trajet trajet_fusionner(Trajet trajet1, Trajet trajet2) {
    if (!trajet1 || !trajet2) return NULL;

    Trajet nouveau_trajet = trajet_init();
    strcpy(nouveau_trajet->gare_depart, trajet1->gare_depart);
    strcpy(nouveau_trajet->gare_arrivee, trajet2->gare_arrivee);
    nouveau_trajet->duree = trajet1->duree + trajet2->duree;

    nouveau_trajet->liste_gares = malloc((trajet1->nombre_gares + trajet2->nombre_gares) * sizeof(char*));
    if (!nouveau_trajet->liste_gares) {
        fprintf(stderr, "Erreur : allocation mémoire pour fusionner les trajets.\n");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < trajet1->nombre_gares; i++) {
        nouveau_trajet->liste_gares[i] = malloc(strlen(trajet1->liste_gares[i]) + 1);
        strcpy(nouveau_trajet->liste_gares[i], trajet1->liste_gares[i]);
    }
    
    for (int i = 0; i < trajet2->nombre_gares; i++) {
        nouveau_trajet->liste_gares[trajet1->nombre_gares + i] = strdup(trajet2->liste_gares[i]);
    }

    nouveau_trajet->nombre_gares = trajet1->nombre_gares + trajet2->nombre_gares;
    
    return nouveau_trajet;
}

Trajet trajet_prolonger(Trajet trajet, char* nouvelle_gare, float temps_supplementaire) {
    if (!trajet || !nouvelle_gare) return NULL;

    strcpy(trajet->gare_arrivee, nouvelle_gare);
    trajet_ajouter_gare(trajet, nouvelle_gare, temps_supplementaire);
    trajet->duree += temps_supplementaire;

    return trajet;
}

Trajet trajet_raccourcir(Trajet trajet, char* gare_finale) {
    if (!trajet || !gare_finale) return NULL;

    for (int i = 0; i < trajet->nombre_gares; i++) {
        if (strcmp(trajet->liste_gares[i], gare_finale) == 0) {
            strcpy(trajet->gare_arrivee, gare_finale);
            for (int j = i + 1; j < trajet->nombre_gares; j++) {
                free(trajet->liste_gares[j]);
            }
            trajet->nombre_gares = i + 1;
            break;
        }
    }

    return trajet;
}


//places libres 
int trajet_places_libres(Trajet trajet) {
    int libres = 0;
    for (int i = 0; i < trajet->nombre_places; i++) {
        if (trajet->places[i] == 0) { // 0 = non réservée
            libres++;
        }
    }
    return libres;
}

int trajet_est_complet(Trajet trajet) {
    return trajet_places_libres(trajet) == 0;
}



// Gestion des fichiers
void trajet_sauvegarder(Trajet trajet, char* nom_fichier) {
    FILE* fichier = fopen(nom_fichier, "w");
    
    if (!fichier || !trajet) {
        fprintf(stderr, "Erreur : impossible de sauvegarder le fichier.\n");
        return;
    }

    fprintf(fichier, "%s\n%s\n%.2f\n%d\n", 
            trajet->gare_depart,
            trajet->gare_arrivee,
            trajet->duree,
            trajet->nombre_gares);

    for (int i = 0; i < trajet->nombre_gares; i++) {
        fprintf(fichier, "%s\n", trajet->liste_gares[i]);
    }

    fclose(fichier);
}

Trajet trajet_charger(char* nom_fichier) {
   FILE *fp=   fopen(nom_fichier, "r");
    if (!fp) {
        fprintf(stderr, "Erreur : impossible d'ouvrir le fichier %s.\n", nom_fichier);
        return NULL;
    }

    Trajet trajet = trajet_init();
    fscanf(fp, "%s\n%s\n%f\n%d\n", 
           trajet->gare_depart, 
           trajet->gare_arrivee, 
           &trajet->duree, 
           &trajet->nombre_gares);

    trajet->liste_gares = malloc(trajet->nombre_gares * sizeof(char*));
    if (!trajet->liste_gares) {
        fprintf(stderr, "Erreur : allocation mémoire pour charger les gares.\n");
        fclose(fp);
        return NULL;
    }

    for (int i = 0; i < trajet->nombre_gares; i++) {
        trajet->liste_gares[i] = malloc(MAX_NOM_VILLE * sizeof(char));
        if (fscanf(fp, "%s\n", trajet->liste_gares[i]) != 1) {
            fprintf(stderr, "Erreur : lecture de la gare échouée.\n");
            free(trajet->liste_gares[i]);
            fclose(fp);
            return NULL;
        }
    }

    fclose(fp);
    return trajet;
}


