#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/trajets.h"

int main() {
    printf("=== TEST TRAJETS ===\n\n");

    // Création de deux trajets
    printf("Initialisation du trajet 1...\n");
    Trajet trajet1 = trajet_init();
    printf("Initialisation du trajet 2...\n");
    Trajet trajet2 = trajet_init();

    // Ajout de gares au trajet 1
    printf("Ajout de Paris à trajet 1 (temps = 0.0)...\n");
    trajet_ajouter_gare(trajet1, "Paris", 0.0);
    printf("Ajout de Lyon à trajet 1 (temps = 2.0)...\n");
    trajet_ajouter_gare(trajet1, "Lyon", 2.0);

    // Ajout de gares au trajet 2
    printf("Ajout de Lyon à trajet 2 (temps = 2.0)...\n");
    trajet_ajouter_gare(trajet2, "Lyon", 2.0);
    printf("Ajout de Marseille à trajet 2 (temps = 3.0)...\n");
    trajet_ajouter_gare(trajet2, "Marseille", 3.0);
    printf("Ajout de Nice à trajet 2 (temps = 4.0)...\n");
    trajet_ajouter_gare(trajet2, "Nice", 4.0);

    // Affichage des trajets
    printf("\n--- Trajet 1 ---\n");
    trajet_afficher(trajet1);
    printf("\n--- Trajet 2 ---\n");
    trajet_afficher(trajet2);

    // Fusion des trajets
    printf("\nFusion des deux trajets...\n");
    Trajet trajet_fusionne = trajet_fusionner(trajet1, trajet2);
    printf("--- Trajet fusionné ---\n");
    trajet_afficher(trajet_fusionne);

    // Prolongement du trajet fusionné
    printf("\nProlongement du trajet fusionné avec Toulouse (temps = 1.5)...\n");
    trajet_prolonger(trajet_fusionne, "Toulouse", 1.5);
    printf("--- Trajet prolongé ---\n");
    trajet_afficher(trajet_fusionne);

    // Raccourcissement du trajet fusionné
    printf("\nRaccourcissement du trajet fusionné à Marseille...\n");
    trajet_raccourcir(trajet_fusionne, "Marseille");
    printf("--- Trajet raccourci ---\n");
    trajet_afficher(trajet_fusionne);

    // Sauvegarde du trajet
    printf("\nSauvegarde du trajet fusionné dans 'trajet.txt'...\n");
    trajet_sauvegarder(trajet_fusionne, "trajet.txt");
    printf("Sauvegarde terminée.\n");

    // Chargement du trajet
    printf("\nChargement du trajet depuis 'trajet.txt'...\n");
    Trajet trajet_charge = trajet_charger("trajet.txt");
    printf("--- Trajet chargé ---\n");
    trajet_afficher(trajet_charge);

    // Libération de la mémoire
    printf("\nLibération de la mémoire...\n");
    trajet_effacer(&trajet1);
    trajet_effacer(&trajet2);
    trajet_effacer(&trajet_fusionne);
    trajet_effacer(&trajet_charge);

    printf("\n=== FIN DES TESTS TRAJETS ===\n");
    return 0;
}
