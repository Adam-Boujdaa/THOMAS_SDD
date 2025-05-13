#include <stdio.h>
#include <stdlib.h>
#include "voyageur.h"
#include "trajet.h"
#include "cJSON.h"
#include "export_db.h"

void create_voyageurs_json(cJSON* root, VoyageurDB* vDB) {
    cJSON* voyageurs_array = cJSON_CreateArray();
    for (int i = 0; i < vDB->nombre_voyageurs; i++) {
        Voyageur v = vDB->voyageurs[i];
        if (v) {
            cJSON* voyageur_json = cJSON_CreateObject();
            cJSON_AddStringToObject(voyageur_json, "nom", voyageur_get_nom(v));
            cJSON_AddNumberToObject(voyageur_json, "identifiant", voyageur_get_identifiant(v));
            cJSON_AddItemToArray(voyageurs_array, voyageur_json);
        }
    }
    cJSON_AddItemToObject(root, "voyageurs", voyageurs_array);
}

void create_trajets_json(cJSON* root, TrajetDB* tDB) {
    cJSON* trajets_array = cJSON_CreateArray();
    for (int i = 0; i < tDB->nombre_trajets; i++) {
        Trajet t = tDB->trajets[i];
        if (t) {
            cJSON* trajet_json = cJSON_CreateObject();
            cJSON_AddStringToObject(trajet_json, "gare_depart", t->gare_depart);
            cJSON_AddStringToObject(trajet_json, "gare_arrivee", t->gare_arrivee);
            cJSON_AddNumberToObject(trajet_json, "duree", t->duree);
            cJSON_AddNumberToObject(trajet_json, "nombre_places", t->nombre_places);
            cJSON_AddNumberToObject(trajet_json, "nombre_reservations", t->nombre_reservations);
            cJSON_AddItemToArray(trajets_array, trajet_json);
        }
    }
    cJSON_AddItemToObject(root, "trajets", trajets_array);
}

void export_db_to_json(const char* filename) {
    cJSON* root = cJSON_CreateObject();

    // Export VoyageurDB
    create_voyageurs_json(root, &voyageurDB);

    // Export TrajetDB
    create_trajets_json(root, &trajetDB);

    // Write JSON to file
    char* json_string = cJSON_Print(root);
    FILE* file = fopen(filename, "w");
    if (file) {
        fprintf(file, "%s", json_string);
        fclose(file);
    } else {
        fprintf(stderr, "Error: Could not open file %s for writing.\n", filename);
    }

    // Clean up
    cJSON_Delete(root);
    free(json_string);
}

void export_voyageurs_to_file(const char* filename) {
    cJSON* root = cJSON_CreateObject();
    create_voyageurs_json(root, &voyageurDB);

    // Write JSON to file
    char* json_string = cJSON_Print(root);

    // Log the generated JSON string
    printf("Generated JSON for voyageurs: %s\n", json_string);

    FILE* file = fopen(filename, "w");
    if (file) {
        fprintf(file, "%s", json_string);
        fclose(file);
    } else {
        fprintf(stderr, "Error: Could not open file %s for writing.\n", filename);
    }

    // Clean up
    cJSON_Delete(root);
    free(json_string);
}

void export_trajets_to_file(const char* filename) {
    cJSON* root = cJSON_CreateObject();
    create_trajets_json(root, &trajetDB);

    // Write JSON to file
    char* json_string = cJSON_Print(root);
    FILE* file = fopen(filename, "w");
    if (file) {
        fprintf(file, "%s", json_string);
        fclose(file);
    } else {
        fprintf(stderr, "Error: Could not open file %s for writing.\n", filename);
    }

    // Clean up
    cJSON_Delete(root);
    free(json_string);
}