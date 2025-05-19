#include "json_export.h"
#include "trajet.h" // For trajetDB
#include "auth.h"   // For users and user_count
#include <stdlib.h>   // For free function
#include <stdio.h>
#include <cjson/cJSON.h>
#include <string.h>

extern VoyageurDB voyageurDB;

void reset_exported_data_json() {
    FILE *file = fopen("data/exported_data.json", "w");
    if (file) {
        fprintf(file, "{\n\t\"trajets\": [],\n\t\"utilisateurs\": []\n}\n");
        fclose(file);
    }
}

void export_trajet_to_json(const Trajet *trajet, const char *filename) {
    FILE *file = fopen(filename, "w");
    if (file) {
        fprintf(file, "{\n");
        fprintf(file, "  \"nom\": \"%s\",\n", trajet->nom);
        fprintf(file, "  \"gares\": [\n");
        for (int i = 0; i < trajet->nombre_gares; i++) {
            fprintf(file, "    {\"nom\": \"%s\", \"temps\": %.1f}%s\n",
                    trajet->gares[i].nom, trajet->gares[i].temps,
                    (i < trajet->nombre_gares - 1) ? "," : "");
        }
        fprintf(file, "  ]\n");
        fprintf(file, "}\n");
        fclose(file);
    }
}

void export_voyageurs_to_json(const Voyageur voyageurs[], int count, const char *filename) {
    FILE *file = fopen(filename, "w");
    if (file) {
        fprintf(file, "[\n");
        for (int i = 0; i < count; i++) {
            fprintf(file, "  {\n");
            fprintf(file, "    \"nom\": \"%s\",\n", voyageurs[i].nom);
            fprintf(file, "    \"identifiant\": \"%s\",\n", voyageurs[i].identifiant);
            fprintf(file, "    \"trajets\": [\n");
            for (int j = 0; j < voyageurs[i].nombre_trajets; j++) {
                fprintf(file, "      {\"nom\": \"%s\"}%s\n",
                        voyageurs[i].trajets[j].nom,
                        (j < voyageurs[i].nombre_trajets - 1) ? "," : "");
            }
            fprintf(file, "    ]\n");
            fprintf(file, "  }%s\n", (i < count - 1) ? "," : "");
        }
        fprintf(file, "]\n");
        fclose(file);
    }
}

void exporter_json(const char *filename) {
    (void)filename;
    cJSON *root = cJSON_CreateObject();
    cJSON *trajets = cJSON_AddArrayToObject(root, "trajets");

    for (int i = 0; i < trajetDB.nombre_trajets; i++) {
        cJSON *trajet = cJSON_CreateObject();
        cJSON_AddStringToObject(trajet, "nom", trajetDB.trajets[i].nom);

        cJSON *gares = cJSON_AddArrayToObject(trajet, "gares");
        for (int j = 0; j < trajetDB.trajets[i].nombre_gares; j++) {
            cJSON *gare = cJSON_CreateObject();
            cJSON_AddStringToObject(gare, "nom", trajetDB.trajets[i].gares[j].nom);
            cJSON_AddNumberToObject(gare, "id", j); // Ajout d'un identifiant unique par arrêt
            if (j > 0) {
                cJSON_AddNumberToObject(gare, "temps", trajetDB.trajets[i].gares[j].temps);
            }
            cJSON_AddItemToArray(gares, gare);
        }

        // Toujours forcer à 10 lors de l'export JSON
        cJSON_AddNumberToObject(trajet, "capacite", 10);
        cJSON_AddNumberToObject(trajet, "places_libres", 10);

        cJSON_AddItemToArray(trajets, trajet);
    }

    cJSON *utilisateurs = cJSON_AddArrayToObject(root, "utilisateurs");
    for (int i = 0; i < voyageurDB.nombre_voyageurs; i++) {
        cJSON *utilisateur = cJSON_CreateObject();
        cJSON_AddStringToObject(utilisateur, "nom_utilisateur", voyageurDB.voyageurs[i].nom);
        cJSON *reservations = cJSON_AddArrayToObject(utilisateur, "reservations");
        for (int r = 0; r < voyageurDB.voyageurs[i].nombre_reservations; r++) {
            cJSON_AddItemToArray(reservations, cJSON_CreateString(voyageurDB.voyageurs[i].reservations[r]));
        }
        cJSON_AddItemToArray(utilisateurs, utilisateur);
    }

    char *json_string = cJSON_Print(root);
    char filepath[100] = "data/exported_data.json";
    FILE *file = fopen(filepath, "w");
    if (file) {
        fprintf(file, "%s", json_string);
        fclose(file);
        printf("Données exportées avec succès dans le fichier %s.\n", filepath);
    } else {
        printf("Erreur : Impossible d'ouvrir le fichier %s pour l'écriture.\n", filepath);
    }

    cJSON_Delete(root);
    free(json_string);
}