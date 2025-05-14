#include "menu.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "login.h"
#include "trajet.h"
#include "export_db.h"

void display_menu() {
    printf("Connectez-vous en tant que:\n");
    printf("1) Administrateur\n");
    printf("2) Controleur\n");
    printf("3) Utilisateur\n");
    printf("\nOU\n\n");
    printf("4) Creer un compte\n");
}

void gerer_choix(int choix) {
    char utilisateur[50], mot_de_passe[50];
    switch (choix) {
        case 1:
            printf("Entrez le mot de passe Administrateur: ");
            scanf("%s", mot_de_passe);
            if (verifier_mot_de_passe_admin(mot_de_passe)) {
                printf("1) Gestion des trajets\n");
                printf("2) Exportation des trajets et des voyageurs\n");
                printf("3) Administration\n");
                printf("8) Quitter le programme\n");
                int choix_admin;
                scanf("%d", &choix_admin);
                switch (choix_admin) {
                    case 1:
                        gerer_trajets();
                        break;
                    case 2:
                        exporter_donnees();
                        break;
                    case 3:
                        changer_mot_de_passe();
                        break;
                    case 8:
                        printf("Fermeture du programme.\n");
                        exit(0);
                    default:
                        printf("Choix invalide.\n");
                }
            } else {
                printf("Mot de passe incorrect.\n");
            }
            break;
        case 2:
            printf("Entrez le mot de passe Controleur: ");
            scanf("%s", mot_de_passe);
            if (verifier_mot_de_passe_controleur(mot_de_passe)) {
                printf("4) Recherche\n");
                printf("5) Administration\n");
                printf("8) Quitter le programme\n");
                int choix_controleur;
                scanf("%d", &choix_controleur);
                switch (choix_controleur) {
                    case 4:
                        rechercher_trajet();
                        break;
                    case 5:
                        mettre_a_jour_infos_controleur();
                        break;
                    case 8:
                        printf("Fermeture du programme.\n");
                        exit(0);
                    default:
                        printf("Choix invalide.\n");
                }
            } else {
                printf("Mot de passe incorrect.\n");
            }
            break;
        case 3:
            printf("Entrez votre nom d'utilisateur: ");
            scanf("%s", utilisateur);
            printf("Entrez votre mot de passe: ");
            scanf("%s", mot_de_passe);
            if (verifier_identifiants_utilisateur(utilisateur, mot_de_passe)) {
                printf("6) Recherche et Choix d’un trajet\n");
                printf("7) Réservation/Modification d’un trajet\n");
                printf("8) Quitter le programme\n");
                int choix_utilisateur;
                scanf("%d", &choix_utilisateur);
                switch (choix_utilisateur) {
                    case 6:
                        rechercher_et_choisir_trajet();
                        break;
                    case 7:
                        reserver_ou_modifier_trajet();
                        break;
                    case 8:
                        printf("Fermeture du programme.\n");
                        exit(0);
                    default:
                        printf("Choix invalide.\n");
                }
            } else {
                printf("Nom d'utilisateur ou mot de passe incorrect.\n");
            }
            break;
        case 4:
            creer_compte();
            printf("6) Recherche et Choix d’un trajet\n");
            printf("7) Réservation/Modification d’un trajet\n");
            printf("8) Quitter le programme\n");
            int choix_nouvel_utilisateur;
            scanf("%d", &choix_nouvel_utilisateur);
            switch (choix_nouvel_utilisateur) {
                case 6:
                    rechercher_et_choisir_trajet();
                    break;
                case 7:
                    reserver_ou_modifier_trajet();
                    break;
                case 8:
                    printf("Fermeture du programme.\n");
                    exit(0);
                default:
                    printf("Choix invalide.\n");
            }
            break;
        case 8:
            printf("Fermeture du programme.\n");
            exit(0);
        default:
            printf("Choix invalide.\n");
            break;
    }
}

void gerer_trajets() {
    printf("Gestion des trajets :\n");
    printf("1) Ajouter une gare\n");
    printf("2) Supprimer une gare\n");
    printf("3) Afficher les trajets\n");
    printf("4) Quitter\n");

    int choix;
    scanf("%d", &choix);

    switch (choix) {
        case 1: {
            char gare[MAX_NOM_VILLE];
            printf("Entrez le nom de la gare à ajouter : ");
            scanf("%s", gare);
            trajet_ajouter_gare(&trajetDB.trajets[0], gare, 0.0); // Exemple avec le premier trajet
            printf("Gare ajoutée avec succès.\n");
            break;
        }
        case 2: {
            char gare[MAX_NOM_VILLE];
            printf("Entrez le nom de la gare à supprimer : ");
            scanf("%s", gare);
            trajet_supprimer_gare(&trajetDB.trajets[0], gare); // Exemple avec le premier trajet
            printf("Gare supprimée avec succès.\n");
            break;
        }
        case 3:
            for (int i = 0; i < trajetDB.nombre_trajets; i++) {
                trajet_afficher(trajetDB.trajets[i]);
            }
            break;
        case 4:
            printf("Retour au menu principal.\n");
            return;
        default:
            printf("Choix invalide.\n");
    }
}