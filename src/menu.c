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
                        export_trajets_to_file();
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

int verifier_mot_de_passe_admin(const char* mot_de_passe) {

}

int verifier_mot_de_passe_controleur(const char* mot_de_passe) {


    return 1; // Placeholder return value
}

int verifier_identifiants_utilisateur(const char* utilisateur, const char* mot_de_passe) {
    // TODO: Implement actual logic
    return 1; // Placeholder return value
}

void rechercher_trajet() {
    char gare_depart[MAX_NOM_VILLE];
    char gare_arrivee[MAX_NOM_VILLE];

    printf("Entrez le nom de la gare de départ : ");
    scanf("%s", gare_depart);

    printf("Entrez le nom de la gare d'arrivée : ");
    scanf("%s", gare_arrivee);

    // Recherche du trajet dans la base globale
    Trajet trouve = NULL;
    for (int i = 0; i < trajetDB.nombre_trajets; i++) {
        Trajet t = trajetDB.trajets[i];
        if (t == NULL) continue;
        if (strcmp(t->gare_depart, gare_depart) == 0 && strcmp(t->gare_arrivee, gare_arrivee) == 0) {
            trouve = t;
            break;
        }
    }

    if (trouve != NULL) {
        printf("\nTrajet trouvé :\n");
        trajet_afficher(trouve);
    } else {
        printf("\nAucun trajet trouvé entre %s et %s.\n", gare_depart, gare_arrivee);
    }
}


#include <stdio.h>
#include <string.h>
#include "trajet.h"

// Fonction utilitaire pour afficher tous les trajets correspondant à une recherche
int afficher_trajets_correspondants(const char* gare_depart, const char* gare_arrivee, int* indices_trouves) {
    int nb_trouves = 0;
    for (int i = 0; i < trajetDB.nombre_trajets; i++) {
        Trajet t = trajetDB.trajets[i];
        if (strcmp(t->gare_depart, gare_depart) == 0 && strcmp(t->gare_arrivee, gare_arrivee) == 0) {
            printf("[%d] Trajet ID %d : %s -> %s, durée %.2f h, places libres : %d\n",
                   nb_trouves + 1, t->id_trajet, t->gare_depart, t->gare_arrivee, t->duree, trajet_places_libres(t));
            indices_trouves[nb_trouves] = i;
            nb_trouves++;
        }
    }
    return nb_trouves;
}

void rechercher_et_choisir_trajet() {
    char gare_depart[MAX_NOM_VILLE];
    char gare_arrivee[MAX_NOM_VILLE];
    int indices[MAX_TRAJETS];

    printf("=== Recherche de trajet ===\n");
    printf("Entrez la gare de départ : ");
    scanf("%s", gare_depart);
    printf("Entrez la gare d'arrivée : ");
    scanf("%s", gare_arrivee);

    int nb_trouves = afficher_trajets_correspondants(gare_depart, gare_arrivee, indices);

    if (nb_trouves == 0) {
        printf("Aucun trajet trouvé entre %s et %s.\n", gare_depart, gare_arrivee);
        return;
    }

    printf("\nEntrez le numéro du trajet à consulter (1-%d), ou 0 pour annuler : ", nb_trouves);
    int choix = 0;
    scanf("%d", &choix);

    if (choix < 1 || choix > nb_trouves) {
        printf("Annulation ou choix invalide.\n");
        return;
    }

    Trajet t = trajetDB.trajets[indices[choix - 1]];
    printf("\n=== Détails du trajet sélectionné ===\n");
    trajet_afficher(t);
}


void reserver_ou_modifier_trajet() {
    int choix_trajet, action, num_place;
    Trajet t;

    if (trajetDB.nombre_trajets == 0) {
        printf("Aucun trajet n'est disponible.\n");
        return;
    }

    afficher_tous_les_trajets();
    printf("Choisissez le numéro du trajet à gérer (0 pour annuler) : ");
    scanf("%d", &choix_trajet);

    if (choix_trajet < 1 || choix_trajet > trajetDB.nombre_trajets) {
        printf("Annulation ou choix invalide.\n");
        return;
    }
    t = trajetDB.trajets[choix_trajet - 1];

    printf("\nDétails du trajet sélectionné :\n");
    trajet_afficher(t);

    printf("\nQue voulez-vous faire ?\n");
    printf("1) Réserver une place\n");
    printf("2) Annuler une réservation\n");
    printf("3) Afficher les places libres\n");
    printf("0) Retour\n");
    printf("Votre choix : ");
    scanf("%d", &action);

    switch (action) {
        case 1:
            if (trajet_est_complet(t)) {
                printf("Ce trajet est complet, aucune place disponible.\n");
                break;
            }
            num_place = trajet_reserver_place(t);
            if (num_place >= 0) {
                printf("Votre réservation est confirmée. Numéro de place : %d\n", num_place + 1);
            } else {
                printf("Erreur lors de la réservation.\n");
            }
            break;
        case 2:
            printf("Entrez le numéro de place à annuler (1 à %d) : ", t->nombre_places);
            scanf("%d", &num_place);
            if (num_place < 1 || num_place > t->nombre_places || t->places[num_place-1] == 0) {
                printf("Numéro de place invalide ou non réservée.\n");
            } else {
                trajet_annuler_place(t, num_place - 1);
                printf("Réservation de la place %d annulée.\n", num_place);
            }
            break;
        case 3:
            trajet_afficher_places_libres(t);
            break;
        case 0:
        default:
            printf("Retour au menu.\n");
            break;
    }
}


extern VoyageurDB voyageurDB;

void creer_compte() {
    char nom_utilisateur[MAX_NOM_UTILISATEUR];
    char mot_de_passe[MAX_MDP];

    printf("=== Création de compte ===\n");

    printf("Choisissez un nom d'utilisateur : ");
    scanf("%s", nom_utilisateur);

    for (int i = 0; i < voyageurDB.nombre_voyageurs; i++) {
        if (strcmp(voyageurDB.voyageurs[i].nom_utilisateur, nom_utilisateur) == 0) {
            printf("Ce nom d'utilisateur existe déjà. Veuillez en choisir un autre.\n");
            return;
        }
    }

    printf("Choisissez un mot de passe : ");
    scanf("%s", mot_de_passe);

r
    if (voyageurDB.nombre_voyageurs >= MAX_VOYAGEURS) {
        printf("Nombre maximal de comptes atteint.\n");
        return;
    }
    strcpy(voyageurDB.voyageurs[voyageurDB.nombre_voyageurs].nom_utilisateur, nom_utilisateur);
    strcpy(voyageurDB.voyageurs[voyageurDB.nombre_voyageurs].mot_de_passe, mot_de_passe);
    voyageurDB.nombre_voyageurs++;

    printf("Compte créé avec succès ! Bienvenue, %s.\n", nom_utilisateur);
}


void changer_mot_de_passe() {
    // TODO: Implement actual logic
    printf("Changement de mot de passe (stub).\n");
}

void mettre_a_jour_infos_controleur() {
    // TODO: Implement actual logic
    printf("Mise à jour des informations du contrôleur (stub).\n");
}
