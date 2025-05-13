#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "voyageur.h"
#include "trajet.h"
#include "export_db.h"
#include "cJSON.h"


void test_export_voyageurs_to_file() {
    const char* filename = "test_voyageurs.json";
    export_voyageurs_to_file(filename);
    printf("Voyageurs exported to %s\n", filename);
}

void test_export_trajets_to_file() {
    const char* filename = "test_trajets.json";
    export_trajets_to_file(filename);
    printf("Trajets exported to %s\n", filename);
}

int main() {
    Voyageur v1 = voyageur_init("Alice", 1, "password1");
    Voyageur v2 = voyageur_init("Bob", 2, "password2");
    voyageurDB.voyageurs[0] = v1;
    voyageurDB.voyageurs[1] = v2;
    voyageurDB.nombre_voyageurs = 2;

    // Initialize some test data for TrajetDB
    Trajet t1 = trajet_init(101, 50);
    strcpy(t1->gare_depart, "Paris");
    strcpy(t1->gare_arrivee, "Lyon");
    t1->duree = 2.5;
    trajetDB.trajets[0] = t1;
    trajetDB.nombre_trajets = 1;
    test_export_voyageurs_to_file();
    test_export_trajets_to_file();
    return 0;
}