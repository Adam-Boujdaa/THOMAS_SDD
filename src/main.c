#include <stdio.h>
#include "trajet.h"
#include "voyageur.h"
#include "place.h"
#include "auth.h"
#include "json_export.h"
#include "menu.h"
#include <stdlib.h>

VoyageurDB voyageurDB = { .nombre_voyageurs = 0 };

int main() {
    // reset_exported_data_json(); // Désactivation de la réinitialisation automatique du JSON
    init_voyageurs();
    load_voyageurs_from_file("voyageur.txt", &voyageurDB);
    importer_trajets_depuis_json("data/exported_data.json");

    int choice;
    do {
        display_menu();
        printf("Votre choix : ");
        scanf("%d", &choice);
        handle_choice(choice);
    } while (choice != 8);

    save_voyageurs_to_file("voyageur.txt", &voyageurDB);
    exporter_json("data/exported_data.json"); // Export automatique à la fermeture
    return 0;
}