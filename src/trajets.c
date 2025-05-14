#define _GNU_SOURCE // Ensure strdup is declared
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "trajet.h"
#include "cJSON.h"

#define strdup(s) ({ \
    const char *_s = (s); \
    char *_d = malloc(strlen(_s) + 1); \
    if (_d) strcpy(_d, _s); \
    _d; \
})

// Define the global TrajetDB instance
TrajetDB trajetDB = { .nombre_trajets = 0 };

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


// Réserve la première place disponible, retourne son numéro ou -1 si complet
int trajet_reserver_place(Trajet trajet) {
    for (int i = 0; i < trajet->nombre_places; i++) {
        if (trajet->places[i] == 0) {
            trajet->places[i] = 1;
            trajet->nombre_reservations++;
            return i;
        }
    }
    return -1;
}


// Annule la réservation d'une place donnée (numéro de place)
void trajet_annuler_place(Trajet trajet, int num_place) {
    if (num_place >= 0 && num_place < trajet->nombre_places && trajet->places[num_place] == 1) {
        trajet->places[num_place] = 0;
        trajet->nombre_reservations--;
    }
}


void trajet_afficher_places_libres(Trajet trajet) {
    printf("Places libres pour le trajet %d : ", trajet->id_trajet);
    for (int i = 0; i < trajet->nombre_places; i++) {
        if (trajet->places[i] == 0) {
            printf("%d ", i);
        }
    }
    printf("\n");
}


void trajet_effacer(Trajet* ptr) {
    if (!ptr || !*ptr) return;
    Trajet trajet = *ptr;
    for (int i = 0; i < trajet->nombre_gares; i++) {
        free(trajet->liste_gares[i]);
    }
    free(trajet->liste_gares);
    free(trajet->places);
    free(trajet);
    *ptr = NULL;
}


void trajet_afficher_places_reservees(Trajet trajet) {
    printf("Places réservées pour le trajet %d : ", trajet->id_trajet);
    for (int i = 0; i < trajet->nombre_places; i++) {
        if (trajet->places[i] == 1) {
            printf("%d ", i);
        }
    }
    printf("\n");
}


void trajet_reinitialiser_reservations(Trajet trajet) {
    for (int i = 0; i < trajet->nombre_places; i++) {
        trajet->places[i] = 0;
    }
    trajet->nombre_reservations = 0;
}


void trajet_afficher(Trajet trajet) {
    printf("Trajet %d : %s -> %s\n", trajet->id_trajet, trajet->gare_depart, trajet->gare_arrivee);
    printf("Durée : %.2f h\n", trajet->duree);
    printf("Gares intermédiaires (%d) : ", trajet->nombre_gares);
    for (int i = 0; i < trajet->nombre_gares; i++) {
        printf("%s ", trajet->liste_gares[i]);
    }
    printf("\nPlaces libres : %d / %d\n", trajet_places_libres(trajet), trajet->nombre_places);
    trajet_afficher_places_libres(trajet);
    trajet_afficher_places_reservees(trajet);
}



// Recherche d'un trajet entre deux gares
Trajet trajet_rechercher(char* gare_depart, char* gare_arrivee) {
    // Cette fonction pourrait être utilisée pour rechercher un trajet entre deux gares.
    // Implémentation dépendante des besoins.
    return NULL;
}
// Calcul de la durée totale du trajet
float trajet_duree_totale(Trajet trajet) {
    if (!trajet) return 0.0;
    return trajet->duree;
}
// Vérifie si le trajet est vide
int trajet_est_vide(Trajet trajet) {
    return trajet->nombre_gares == 0 && trajet->duree == 0.0;
}
// Retourne le nombre de gares dans le trajet
int trajet_nombre_gares(Trajet trajet) {
    return trajet->nombre_gares;
}

//Retourne l'ID du trajet
int trajet_get_id(Trajet trajet) {
    return trajet->id_trajet;
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

    Trajet trajet = trajet_init(0, 0); // Provide default arguments
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

Trajet trajet_fusionner(Trajet trajet1, Trajet trajet2) {
    if (!trajet1 || !trajet2) return NULL;

    // Create a new Trajet for the fusion
    Trajet fusion = trajet_init(trajet1->id_trajet, trajet1->nombre_places);

    // Copy gares from trajet1
    for (int i = 0; i < trajet1->nombre_gares; i++) {
        trajet_ajouter_gare(fusion, trajet1->liste_gares[i], 0.0); // Adjust time as needed
    }

    // Copy gares from trajet2
    for (int i = 0; i < trajet2->nombre_gares; i++) {
        trajet_ajouter_gare(fusion, trajet2->liste_gares[i], 0.0); // Adjust time as needed
    }

    // Update metadata
    strcpy(fusion->gare_depart, trajet1->gare_depart);
    strcpy(fusion->gare_arrivee, trajet2->gare_arrivee);
    fusion->duree = trajet1->duree + trajet2->duree;

    return fusion;
}

// Function to load trajets from a JSON file
Trajet charger_trajets(const char* fichier_json, int* nombre_trajets) {
    FILE* fichier = fopen(fichier_json, "r");
    if (!fichier) {
        fprintf(stderr, "Erreur : Impossible d'ouvrir le fichier %s\n", fichier_json);
        return NULL;
    }

    // Lire le fichier entier dans un buffer
    fseek(fichier, 0, SEEK_END);
    long taille_fichier = ftell(fichier);
    rewind(fichier);

    char* buffer = (char*)malloc(taille_fichier + 1);
    if (!buffer) {
        fprintf(stderr, "Erreur : Mémoire insuffisante pour lire le fichier\n");
        fclose(fichier);
        return NULL;
    }

    fread(buffer, 1, taille_fichier, fichier);
    buffer[taille_fichier] = '\0';
    fclose(fichier);

    // Parser les données JSON
    cJSON* json = cJSON_Parse(buffer);
    free(buffer);

    if (!json) {
        fprintf(stderr, "Erreur : Échec de l'analyse du fichier JSON\n");
        return NULL;
    }

    cJSON* trajets_array = cJSON_GetObjectItem(json, "trajets");
    if (!cJSON_IsArray(trajets_array)) {
        fprintf(stderr, "Erreur : Le fichier JSON ne contient pas de tableau de trajets\n");
        cJSON_Delete(json);
        return NULL;
    }

    *nombre_trajets = cJSON_GetArraySize(trajets_array);
    Trajet trajets = malloc(*nombre_trajets * sizeof(struct s_trajet));
    if (!trajets) {
        fprintf(stderr, "Erreur : Mémoire insuffisante pour les trajets\n");
        cJSON_Delete(json);
        return NULL;
    }

    for (int i = 0; i < *nombre_trajets; i++) {
        cJSON* trajet_json = cJSON_GetArrayItem(trajets_array, i);
        cJSON* gare_depart = cJSON_GetObjectItem(trajet_json, "gare_depart");
        cJSON* gare_arrivee = cJSON_GetObjectItem(trajet_json, "gare_arrivee");
        cJSON* duree = cJSON_GetObjectItem(trajet_json, "duree");
        cJSON* nombre_places = cJSON_GetObjectItem(trajet_json, "nombre_places");
        cJSON* nombre_reservations = cJSON_GetObjectItem(trajet_json, "nombre_reservations");
        cJSON* liste_gares = cJSON_GetObjectItem(trajet_json, "liste_gares");

        if (cJSON_IsString(gare_depart) && cJSON_IsString(gare_arrivee) && cJSON_IsNumber(duree) &&
            cJSON_IsNumber(nombre_places) && cJSON_IsNumber(nombre_reservations) && cJSON_IsArray(liste_gares)) {

            strncpy(trajets[i].gare_depart, gare_depart->valuestring, MAX_NOM_VILLE - 1);
            trajets[i].gare_depart[MAX_NOM_VILLE - 1] = '\0';

            strncpy(trajets[i].gare_arrivee, gare_arrivee->valuestring, MAX_NOM_VILLE - 1);
            trajets[i].gare_arrivee[MAX_NOM_VILLE - 1] = '\0';

            trajets[i].duree = (float)duree->valuedouble;
            trajets[i].nombre_places = nombre_places->valueint;
            trajets[i].nombre_reservations = nombre_reservations->valueint;

            int nombre_gares = cJSON_GetArraySize(liste_gares);
            trajets[i].nombre_gares = nombre_gares;
            trajets[i].liste_gares = (char**)malloc(nombre_gares * sizeof(char*));

            for (int j = 0; j < nombre_gares; j++) {
                cJSON* gare = cJSON_GetArrayItem(liste_gares, j);
                if (cJSON_IsString(gare)) {
                    trajets[i].liste_gares[j] = strdup(gare->valuestring);
                }
            }

            trajets[i].places = (int*)malloc(trajets[i].nombre_places * sizeof(int));
            for (int k = 0; k < trajets[i].nombre_places; k++) {
                trajets[i].places[k] = 0; // Initialiser toutes les places comme non réservées
            }

            trajets[i].id_trajet = i + 1; // Générer un identifiant unique pour chaque trajet
        }
    }

    cJSON_Delete(json);
    return trajets;
}


