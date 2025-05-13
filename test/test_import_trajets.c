#include <stdio.h>
#include <stdlib.h>
#include "trajet.h"

int main() {
    const char* fichier_test = "test_trajets.json";
    int nombre_trajets = 0;

    // Charger les trajets depuis le fichier JSON
    Trajet* trajets = charger_trajets(fichier_test, &nombre_trajets);

    if (trajets == NULL) {
        fprintf(stderr, "Erreur : Impossible de charger les trajets depuis le fichier %s\n", fichier_test);
        return EXIT_FAILURE;
    }

    // Vérifier les trajets chargés
    printf("Nombre de trajets chargés : %d\n", nombre_trajets);
    for (int i = 0; i < nombre_trajets; i++) {
        printf("Trajet %d : %s -> %s, Durée : %.2f heures, Places : %d\n",
               i + 1,
               trajets[i].gare_depart,
               trajets[i].gare_arrivee,
               trajets[i].duree,
               trajets[i].nombre_places);
    }

    // Libérer la mémoire allouée
    for (int i = 0; i < nombre_trajets; i++) {
        free(trajets[i].gares);
    }
    free(trajets);

    return EXIT_SUCCESS;
}