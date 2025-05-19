#include "menu.h"
#include "auth.h"   // Ensure consistent declaration of changer_mot_de_passe
#include "login.h"
#include "trajet.h" // Ensure modifier_trajet_interactif is declared
#include <stdio.h>
#include <string.h>
#include <stdlib.h> // For system("read -p")
#include <stdbool.h>
#include <ctype.h>
#include <ctype.h> // pour isspace()
#include <strings.h> // pour strcasecmp
// --- Ajout pour recherche sur fichier texte ---
#define MAX_TRAJETS_TXT 100
#define MAX_GARES_TXT 20
#define MAX_NOM_TXT 50

typedef struct {
    char nom[MAX_NOM_TXT];
    char gares[MAX_GARES_TXT][MAX_NOM_TXT];
    float temps[MAX_GARES_TXT]; // Ajout du temps pour chaque gare
    int nb_gares;
} TrajetSimple;

#define ADMIN_FILE "admin.txt"
#define CONTROLEUR_FILE "controleur.txt"

extern TrajetDB trajetDB; // Declare the global variable to access it in this file
extern VoyageurDB voyageurDB; // Declare the global variable for VoyageurDB

// Ajout d'une variable statique pour suivre l'utilisateur connecté
static char utilisateur_connecte[MAX_USERNAME_LENGTH] = "";

// Déclaration explicite en haut du fichier
void afficher_trajets(void);
void exporter_json(const char *filename);
void creer_compte_interactif(void);
void rechercher_et_reserver_trajet();

// Fonction utilitaire pour lire identifiant/mot de passe depuis un fichier
static bool lire_identifiants(const char *filename, char *identifiant, char *mot_de_passe) {
    FILE *file = fopen(filename, "r");
    if (!file) return false;
    if (fscanf(file, "%49s %49s", identifiant, mot_de_passe) == 2) {
        fclose(file);
        return true;
    }
    fclose(file);
    return false;
}

// Fonction utilitaire pour demander identifiant/mot de passe à l'utilisateur
static void demander_identifiants(char *identifiant, char *mot_de_passe) {
    printf("Identifiant : ");
    scanf("%49s", identifiant);
    getchar();
    printf("Mot de passe : ");
    scanf("%49s", mot_de_passe);
    getchar();
}

// Authentification admin
static bool authentifier_admin() {
    char id[50], mdp[50], id_file[50], mdp_file[50];
    if (!lire_identifiants(ADMIN_FILE, id_file, mdp_file)) {
        printf("Fichier admin.txt manquant ou invalide.\n");
        return false;
    }
    demander_identifiants(id, mdp);
    if (strcmp(id, id_file) == 0 && strcmp(mdp, mdp_file) == 0) return true;
    printf("Identifiant ou mot de passe administrateur incorrect.\n");
    return false;
}

// Authentification controleur
static bool authentifier_controleur() {
    char id[50], mdp[50], id_file[50], mdp_file[50];
    if (!lire_identifiants(CONTROLEUR_FILE, id_file, mdp_file)) {
        printf("Fichier controleur.txt manquant ou invalide.\n");
        return false;
    }
    demander_identifiants(id, mdp);
    if (strcmp(id, id_file) == 0 && strcmp(mdp, mdp_file) == 0) return true;
    printf("Identifiant ou mot de passe contrôleur incorrect.\n");
    return false;
}

// Authentification utilisateur (création de compte possible)
static bool authentifier_user() {
    char id[50], mdp[50], nom[50];
    printf("1. Connexion\n2. Créer un compte\nVotre choix : ");
    int choix = 0;
    scanf("%d", &choix);
    getchar();
    if (choix == 2) {
        printf("Nom : ");
        scanf("%49s", nom);
        getchar();
        printf("Identifiant : ");
        scanf("%49s", id);
        getchar();
        printf("Mot de passe : ");
        scanf("%49s", mdp);
        getchar();
        if (creer_voyageur(id, mdp, nom, &voyageurDB)) {
            save_voyageurs_to_file("voyageur.txt", &voyageurDB);
            printf("Compte créé avec succès.\n");
        } else {
            printf("Erreur : identifiant déjà utilisé ou base pleine.\n");
        }
        return false;
    }
    demander_identifiants(id, mdp);
    if (authenticate_voyageur(id, mdp, &voyageurDB)) {
        strncpy(utilisateur_connecte, id, MAX_USERNAME_LENGTH);
        return true;
    }
    printf("Identifiant ou mot de passe utilisateur incorrect.\n");
    return false;
}

void pause_for_user() {
    printf("Appuyez sur Entrée pour continuer...\n");
    while (getchar() != '\n'); // Wait for Enter key
}

// Renamed the local changer_mot_de_passe function to avoid conflict
void changer_mot_de_passe_local() {
    char username[MAX_USERNAME_LENGTH];
    char new_password[MAX_PASSWORD_LENGTH];

    printf("Entrez le nom d'utilisateur : ");
    scanf("%s", username);
    getchar(); // Consume newline character

    printf("Entrez le nouveau mot de passe : ");
    scanf("%s", new_password);
    getchar(); // Consume newline character

    if (change_password_voyageur(username, new_password, &voyageurDB)) {
        save_voyageurs_to_file("voyageur.txt", &voyageurDB);
        printf("Mot de passe changé avec succès.\n");
    } else {
        printf("Erreur : utilisateur introuvable.\n");
    }
    pause_for_user();
}

void rechercher_trajet() {
    char gare_depart[50], gare_arrivee[50];

    printf("Entrez la gare de départ : ");
    scanf("%s", gare_depart);
    getchar(); // Consume newline character

    printf("Entrez la gare d'arrivée : ");
    scanf("%s", gare_arrivee);
    getchar(); // Consume newline character

    Trajet trajet = trajet_rechercher(gare_depart, gare_arrivee);
    if (trajet.nombre_gares > 0) {
        printf("Trajet trouvé :\n");
        trajet_afficher(&trajet);
    } else {
        printf("Aucun trajet trouvé entre %s et %s.\n", gare_depart, gare_arrivee);
    }
    pause_for_user();
}

void rechercher_et_choisir_trajet() {
    if (trajetDB.nombre_trajets == 0) {
        printf("Aucun trajet disponible.\n");
        pause_for_user();
        return;
    }
    afficher_trajets();
    char nom[50];
    printf("Entrez le nom EXACT du trajet à consulter : ");
    fflush(stdout);
    if (scanf("%49s", nom) != 1) {
        printf("Erreur de saisie.\n");
        pause_for_user();
        return;
    }
    getchar();
    int trouve = 0;
    for (int i = 0; i < trajetDB.nombre_trajets; i++) {
        if (strcmp(trajetDB.trajets[i].nom, nom) == 0) {
            printf("Détails du trajet '%s' :\n", nom);
            trajet_afficher(&trajetDB.trajets[i]);
            trouve = 1;
            break;
        }
    }
    if (!trouve) {
        printf("Erreur : Trajet '%s' non trouvé.\n", nom);
    }
    pause_for_user();
}

void reserver_ou_modifier_trajet() {
    if (trajetDB.nombre_trajets == 0) {
        printf("Aucun trajet disponible pour réservation.\n");
        pause_for_user();
        return;
    }
    afficher_trajets();
    char nom[50];
    printf("Entrez le nom EXACT du trajet à réserver : ");
    fflush(stdout);
    if (scanf("%49s", nom) != 1) {
        printf("Erreur de saisie.\n");
        pause_for_user();
        return;
    }
    getchar();
    int trouve = 0;
    for (int i = 0; i < trajetDB.nombre_trajets; i++) {
        if (strcmp(trajetDB.trajets[i].nom, nom) == 0) {
            if (trajetDB.trajets[i].places_libres > 0) {
                trajetDB.trajets[i].places_libres--;
                // Ajout de la réservation au tableau de l'utilisateur connecté
                // Recherche du voyageur correspondant
                for (int v = 0; v < voyageurDB.nombre_voyageurs; v++) {
                    if (strcmp(voyageurDB.voyageurs[v].nom, utilisateur_connecte) == 0) {
                        if (voyageurDB.voyageurs[v].nombre_reservations < MAX_TRAJETS) {
                            strncpy(voyageurDB.voyageurs[v].reservations[voyageurDB.voyageurs[v].nombre_reservations], nom, 50);
                            voyageurDB.voyageurs[v].nombre_reservations++;
                        }
                        break;
                    }
                }
                printf("Réservation effectuée sur '%s'. Places restantes : %d\n", nom, trajetDB.trajets[i].places_libres);
                exporter_json("data/exported_data.json");
            } else {
                printf("Aucune place disponible sur ce trajet.\n");
            }
            trouve = 1;
            break;
        }
    }
    if (!trouve) {
        printf("Erreur : Trajet '%s' non trouvé.\n", nom);
    }
    pause_for_user();
}

// Fonction utilitaire pour exporter tous les trajets dans trajet.txt (format nom puis gares)
void exporter_trajets_txt() {
    FILE *f = fopen("trajet.txt", "w");
    if (!f) return;
    for (int i = 0; i < trajetDB.nombre_trajets; i++) {
        fprintf(f, "%s", trajetDB.trajets[i].nom);
        for (int g = 0; g < trajetDB.trajets[i].nombre_gares; g++) {
            fprintf(f, " %s %.1f", trajetDB.trajets[i].gares[g].nom, trajetDB.trajets[i].gares[g].temps);
        }
        // Ajout du nombre de places libres à la fin de la ligne
        fprintf(f, " PLACES_LIBRES %d", trajetDB.trajets[i].places_libres);
        fprintf(f, "\n");
    }
    fclose(f);
}

int contient_espace(const char *str) {
    while (*str) {
        if (isspace((unsigned char)*str)) return 1;
        str++;
    }
    return 0;
}

void ajouter_nouveau_trajet() {
    if (trajetDB.nombre_trajets >= MAX_TRAJETS) {
        printf("Erreur : Limite de trajets atteinte (%d).\n", MAX_TRAJETS);
        return;
    }
    Trajet new_trajet;
    char gare_nom[50];
    float temps;

    // Saisie du nom du trajet sans espaces
    while (1) {
        printf("Entrez le nom du trajet (syntaxe : GareDepart-GareArrivee, sans espaces) : ");
        char buffer[100];
        fgets(buffer, sizeof(buffer), stdin);
        buffer[strcspn(buffer, "\n")] = '\0';
        // Vérification : le nom ne doit pas contenir d'espace et ne doit pas être vide
        if (strchr(buffer, ' ') != NULL || strlen(buffer) == 0) {
            printf("Erreur : le nom du trajet ne doit pas contenir d'espace et ne doit pas être vide. Retour au menu gestion des trajets.\n");
            return;
        }
        strncpy(new_trajet.nom, buffer, sizeof(new_trajet.nom));
        break;
    }

    new_trajet.nombre_gares = 0;

    // Première gare (départ)
    printf("Entrez le nom de la gare de départ : ");
    scanf("%s", gare_nom);
    getchar();
    trajet_ajouter_gare(&new_trajet, gare_nom, 0.0f); // temps = 0 pour la première gare

    // Gares suivantes
    while (1) {
        if (new_trajet.nombre_gares >= MAX_GARES) {
            printf("Nombre maximum de gares atteint (%d).\n", MAX_GARES);
            break;
        }
        printf("Entrez le nom de la gare (ou 'fin' pour terminer) : ");
        scanf("%s", gare_nom);
        getchar();
        if (strcmp(gare_nom, "fin") == 0) {
            break;
        }
        printf("Entrez le temps pour atteindre cette gare : ");
        scanf("%f", &temps);
        getchar();
        trajet_ajouter_gare(&new_trajet, gare_nom, temps);
        printf("Ajout gare %s (%.1f) au trajet %s (total gares: %d)\n", gare_nom, temps, new_trajet.nom, new_trajet.nombre_gares);
    }

    trajetDB.trajets[trajetDB.nombre_trajets++] = new_trajet;
    printf("Trajet ajouté avec succès ! (total trajets: %d)\n", trajetDB.nombre_trajets);
    exporter_json("data/exported_data.json");
    exporter_trajets_txt();
}


void supprimer_trajet_par_nom() {
    if (trajetDB.nombre_trajets == 0) {
        printf("Aucun trajet à supprimer.\n");
        pause_for_user();
        return;
    }
    afficher_trajets();
    char nom[50];
    printf("Entrez le nom EXACT du trajet à supprimer : ");
    fflush(stdout);
    if (scanf("%49s", nom) != 1) {
        printf("Erreur de saisie.\n");
        pause_for_user();
        return;
    }
    getchar();
    int trouve = 0;
    for (int i = 0; i < trajetDB.nombre_trajets; i++) {
        if (strcmp(trajetDB.trajets[i].nom, nom) == 0) {
            // Décaler tous les trajets suivants
            for (int j = i; j < trajetDB.nombre_trajets - 1; j++) {
                trajetDB.trajets[j] = trajetDB.trajets[j + 1];
            }
            trajetDB.nombre_trajets--;
            trouve = 1;
            printf("Trajet '%s' supprimé avec succès.\n", nom);
            exporter_json("data/exported_data.json");
            exporter_trajets_txt(); // MAJ automatique du fichier texte
            break;
        }
    }
    if (!trouve) {
        printf("Erreur : Trajet '%s' non trouvé.\n", nom);
    }
    pause_for_user();
}

void afficher_trajets() {
    printf("Liste des trajets disponibles :\n");
    for (int i = 0; i < trajetDB.nombre_trajets; i++) {
        printf("- %s\n", trajetDB.trajets[i].nom);
    }
}

void consulter_mes_reservations() {
    int found = 0;
    printf("Vos réservations :\n");
    for (int v = 0; v < voyageurDB.nombre_voyageurs; v++) {
        if (strcmp(voyageurDB.voyageurs[v].identifiant, utilisateur_connecte) == 0) {
            for (int r = 0; r < voyageurDB.voyageurs[v].nombre_reservations; r++) {
                char *nom_trajet = voyageurDB.voyageurs[v].reservations[r];
                printf("- %s\n", nom_trajet);
                // Afficher les détails du trajet
                int trouve_trajet = 0;
                for (int t = 0; t < trajetDB.nombre_trajets; t++) {
                    if (strcmp(trajetDB.trajets[t].nom, nom_trajet) == 0) {
                        trajet_afficher(&trajetDB.trajets[t]);
                        trouve_trajet = 1;
                        break;
                    }
                }
                if (!trouve_trajet) {
                    printf("  [Trajet non trouvé ou supprimé]\n");
                }
                found = 1;
            }
            break;
        }
    }
    if (!found) {
        printf("Aucune réservation trouvée.\n");
    }
    pause_for_user();
}

void gestion_trajets() {
    int choix;
    do {
        printf("=== Gestion des trajets ===\n");
        printf("1. Ajouter un trajet\n");
        printf("2. Modifier un trajet\n");
        printf("3. Supprimer un trajet par nom\n");
        printf("4. Retour au menu administrateur\n");
        printf("Votre choix : ");
        scanf("%d", &choix);
        getchar(); // Consume newline character

        switch (choix) {
            case 1:
                printf("Ajout d'un trajet...\n");
                ajouter_nouveau_trajet();
                pause_for_user();
                break;
            case 2:
                printf("Modification d'un trajet...\n");
                modifier_trajet_interactif();
                pause_for_user();
                break;
            case 3:
                supprimer_trajet_par_nom();
                break;
            case 4:
                printf("Retour au menu administrateur...\n");
                break;
            default:
                printf("Choix invalide.\n");
                pause_for_user();
        }
    } while (choix != 4);
}

void rechercher_trajet_txt() {
    TrajetSimple trajets[MAX_TRAJETS_TXT];
    int nb_trajets = 0;
    FILE *f = fopen("trajet.txt", "r");
    if (!f) {
        printf("Impossible d'ouvrir trajet.txt\n");
        return;
    }
    char ligne[512];
    while (fgets(ligne, sizeof(ligne), f) && nb_trajets < MAX_TRAJETS_TXT) {
        char *token = strtok(ligne, " \n");
        int g = 0;
        if (token) {
            strncpy(trajets[nb_trajets].nom, token, MAX_NOM_TXT);
            token = strtok(NULL, " \n");
        } else {
            trajets[nb_trajets].nom[0] = '\0';
        }
        while (token && g < MAX_GARES_TXT) {
            strncpy(trajets[nb_trajets].gares[g], token, MAX_NOM_TXT);
            token = strtok(NULL, " \n");
            if (token) {
                trajets[nb_trajets].temps[g] = atof(token);
                token = strtok(NULL, " \n");
            } else {
                trajets[nb_trajets].temps[g] = 0.0f;
            }
            g++;
        }
        trajets[nb_trajets].nb_gares = g;
        nb_trajets++;
    }
    fclose(f);
    char depart[MAX_NOM_TXT], arrivee[MAX_NOM_TXT];
    printf("Gare de départ : ");
    scanf("%49s", depart);
    printf("Gare d'arrivée : ");
    scanf("%49s", arrivee);
    int found = 0;
    int deja_reserve = 0;
    // Recherche trajet direct
    for (int t = 0; t < nb_trajets && !deja_reserve; t++) {
        int idx_dep = -1, idx_arr = -1;
        for (int g = 0; g < trajets[t].nb_gares; g++) {
            if (strcmp(trajets[t].gares[g], depart) == 0) idx_dep = g;
            if (strcmp(trajets[t].gares[g], arrivee) == 0) idx_arr = g;
        }
        if (idx_dep != -1 && idx_arr != -1 && idx_dep < idx_arr) {
            float temps_total = 0.0f;
            for (int g = idx_dep + 1; g <= idx_arr; g++) {
                temps_total += trajets[t].temps[g];
            }
            printf("Trajet direct trouvé : %s (de %s à %s) | Temps total : %.1f h\n", trajets[t].nom, depart, arrivee, temps_total);
            found = 1;
            printf("Voulez-vous réserver ce trajet ? (1=Oui, 0=Non) : ");
            int rep = 0;
            scanf("%d", &rep);
            getchar();
            if (rep == 1) {
                int reserve = 0;
                for (int i = 0; i < trajetDB.nombre_trajets; i++) {
                    if (strcmp(trajetDB.trajets[i].nom, trajets[t].nom) == 0 && trajetDB.trajets[i].places_libres > 0) {
                        trajetDB.trajets[i].places_libres--;
                        for (int v = 0; v < voyageurDB.nombre_voyageurs; v++) {
                            if (strcmp(voyageurDB.voyageurs[v].nom, utilisateur_connecte) == 0) {
                                if (voyageurDB.voyageurs[v].nombre_reservations < MAX_TRAJETS) {
                                    strncpy(voyageurDB.voyageurs[v].reservations[voyageurDB.voyageurs[v].nombre_reservations], trajets[t].nom, 50);
                                    voyageurDB.voyageurs[v].nombre_reservations++;
                                }
                                break;
                            }
                        }
                        printf("Réservation effectuée sur '%s'. Places restantes : %d\n", trajets[t].nom, trajetDB.trajets[i].places_libres);
                        exporter_json("data/exported_data.json");
                        reserve = 1;
                        break;
                    }
                }
                if (!reserve) {
                    printf("Aucune place disponible sur ce trajet ou trajet introuvable.\n");
                }
                deja_reserve = 1;
                break;
            }
        }
    }
    // Recherche correspondance simple
    for (int t1 = 0; t1 < nb_trajets && !deja_reserve; t1++) {
        int idx_dep = -1;
        for (int g = 0; g < trajets[t1].nb_gares; g++)
            if (strcmp(trajets[t1].gares[g], depart) == 0) idx_dep = g;
        if (idx_dep == -1) continue;
        for (int g1 = idx_dep+1; g1 < trajets[t1].nb_gares && !deja_reserve; g1++) {
            char *correspondance = trajets[t1].gares[g1];
            if (strcmp(correspondance, arrivee) == 0) {
                float temps_total = 0.0f;
                for (int g = idx_dep + 1; g <= g1; g++) temps_total += trajets[t1].temps[g];
                printf("Trajet direct trouvé (via correspondance) : %s (de %s à %s) | Temps total : %.1f h\n", trajets[t1].nom, depart, arrivee, temps_total);
                found = 1;
                printf("Voulez-vous réserver ce trajet ? (1=Oui, 0=Non) : ");
                int rep = 0;
                scanf("%d", &rep);
                getchar();
                if (rep == 1) {
                    int reserve = 0;
                    for (int i = 0; i < trajetDB.nombre_trajets; i++) {
                        if (strcmp(trajetDB.trajets[i].nom, trajets[t1].nom) == 0 && trajetDB.trajets[i].places_libres > 0) {
                            trajetDB.trajets[i].places_libres--;
                            for (int v = 0; v < voyageurDB.nombre_voyageurs; v++) {
                                if (strcmp(voyageurDB.voyageurs[v].nom, utilisateur_connecte) == 0) {
                                    if (voyageurDB.voyageurs[v].nombre_reservations < MAX_TRAJETS) {
                                        strncpy(voyageurDB.voyageurs[v].reservations[voyageurDB.voyageurs[v].nombre_reservations], trajets[t1].nom, 50);
                                        voyageurDB.voyageurs[v].nombre_reservations++;
                                    }
                                    break;
                                }
                            }
                            printf("Réservation effectuée sur '%s'. Places restantes : %d\n", trajets[t1].nom, trajetDB.trajets[i].places_libres);
                            reserve = 1;
                            break;
                        }
                    }
                    if (!reserve) {
                        printf("Aucune place disponible sur ce trajet ou trajet introuvable.\n");
                    }
                    deja_reserve = 1;
                    break;
                }
            }
            for (int t2 = 0; t2 < nb_trajets && !deja_reserve; t2++) {
                if (t1 == t2) continue;
                int idx_cor = -1, idx_arr = -1;
                for (int g2 = 0; g2 < trajets[t2].nb_gares; g2++) {
                    if (strcmp(trajets[t2].gares[g2], correspondance) == 0) idx_cor = g2;
                    if (strcmp(trajets[t2].gares[g2], arrivee) == 0) idx_arr = g2;
                }
                if (idx_cor != -1 && idx_arr != -1 && idx_cor < idx_arr) {
                    // Calcul du temps sur t1 : de depart à cor
                    float temps1 = 0.0f;
                    for (int g = idx_dep + 1; g <= g1; g++) temps1 += trajets[t1].temps[g];
                    // Calcul du temps sur t2 : de cor à arrivée
                    float temps2 = 0.0f;
                    for (int g = idx_cor + 1; g <= idx_arr; g++) temps2 += trajets[t2].temps[g];
                    if (temps1 > 0.0f && temps2 > 0.0f) {
                        float temps_total = temps1 + temps2;
                        printf("Correspondance : %s (de %s à %s, %.1f h), puis %s (de %s à %s, %.1f h) | Temps total : %.1f h\n",
                            trajets[t1].nom, depart, correspondance, temps1,
                            trajets[t2].nom, correspondance, arrivee, temps2,
                            temps_total);
                        found = 1;
                        printf("Voulez-vous réserver cette correspondance ? (1=Oui, 0=Non) : ");
                        int rep = 0;
                        scanf("%d", &rep);
                        getchar();
                        if (rep == 1) {
                            int reserve1 = 0, reserve2 = 0;
                            for (int i = 0; i < trajetDB.nombre_trajets; i++) {
                                if (strcmp(trajetDB.trajets[i].nom, trajets[t1].nom) == 0 && trajetDB.trajets[i].places_libres > 0) {
                                    trajetDB.trajets[i].places_libres--;
                                    for (int v = 0; v < voyageurDB.nombre_voyageurs; v++) {
                                        if (strcmp(voyageurDB.voyageurs[v].nom, utilisateur_connecte) == 0) {
                                            if (voyageurDB.voyageurs[v].nombre_reservations < MAX_TRAJETS) {
                                                strncpy(voyageurDB.voyageurs[v].reservations[voyageurDB.voyageurs[v].nombre_reservations], trajets[t1].nom, 50);
                                                voyageurDB.voyageurs[v].nombre_reservations++;
                                            }
                                            break;
                                        }
                                    }
                                    reserve1 = 1;
                                    break;
                                }
                            }
                            for (int i = 0; i < trajetDB.nombre_trajets; i++) {
                                if (strcmp(trajetDB.trajets[i].nom, trajets[t2].nom) == 0 && trajetDB.trajets[i].places_libres > 0) {
                                    trajetDB.trajets[i].places_libres--;
                                    for (int v = 0; v < voyageurDB.nombre_voyageurs; v++) {
                                        if (strcmp(voyageurDB.voyageurs[v].nom, utilisateur_connecte) == 0) {
                                            if (voyageurDB.voyageurs[v].nombre_reservations < MAX_TRAJETS) {
                                                strncpy(voyageurDB.voyageurs[v].reservations[voyageurDB.voyageurs[v].nombre_reservations], trajets[t2].nom, 50);
                                                voyageurDB.voyageurs[v].nombre_reservations++;
                                            }
                                            break;
                                        }
                                    }
                                    reserve2 = 1;
                                    break;
                                }
                            }
                            if (reserve1 && reserve2) {
                                printf("Réservation effectuée sur '%s' et '%s'.\n", trajets[t1].nom, trajets[t2].nom);
                                exporter_json("data/exported_data.json");
                            } else {
                                printf("Aucune place disponible sur l'un des trajets ou trajets introuvables.\n");
                            }
                            deja_reserve = 1;
                            break;
                        }
                    }
                }
            }
        }
    }
    if (!found) {
        printf("Aucun trajet direct ou avec correspondance trouvé.\n");
    }
    pause_for_user();
}

// Recherche trajet pour le contrôleur (lecture depuis trajet.txt, sans réservation)
void rechercher_trajet_txt_controleur() {
    TrajetSimple trajets[MAX_TRAJETS_TXT];
    int nb_trajets = 0;
    FILE *f = fopen("trajet.txt", "r");
    if (!f) {
        printf("Impossible d'ouvrir trajet.txt\n");
        return;
    }
    char ligne[512];
    while (fgets(ligne, sizeof(ligne), f) && nb_trajets < MAX_TRAJETS_TXT) {
        char *token = strtok(ligne, " \n");
        int g = 0;
        if (token) {
            strncpy(trajets[nb_trajets].nom, token, MAX_NOM_TXT);
            token = strtok(NULL, " \n");
        } else {
            trajets[nb_trajets].nom[0] = '\0';
        }
        while (token && g < MAX_GARES_TXT) {
            strncpy(trajets[nb_trajets].gares[g], token, MAX_NOM_TXT);
            token = strtok(NULL, " \n");
            if (token) {
                trajets[nb_trajets].temps[g] = atof(token);
                token = strtok(NULL, " \n");
            } else {
                trajets[nb_trajets].temps[g] = 0.0f;
            }
            g++;
        }
        trajets[nb_trajets].nb_gares = g;
        nb_trajets++;
    }
    fclose(f);
    char depart[MAX_NOM_TXT], arrivee[MAX_NOM_TXT];
    printf("Gare de départ : ");
    scanf("%49s", depart);
    printf("Gare d'arrivée : ");
    scanf("%49s", arrivee);
    // int found = 0; // supprimé car inutilisé dans cette fonction
    // Trajets directs
    for (int t = 0; t < nb_trajets; t++) {
        int idx_dep = -1, idx_arr = -1;
        for (int g = 0; g < trajets[t].nb_gares; g++) {
            if (strcmp(trajets[t].gares[g], depart) == 0) idx_dep = g;
            if (strcmp(trajets[t].gares[g], arrivee) == 0) idx_arr = g;
        }
        if (idx_dep != -1 && idx_arr != -1 && idx_dep < idx_arr) {
            float temps_total = 0.0f;
            for (int g = idx_dep + 1; g <= idx_arr; g++) {
                temps_total += trajets[t].temps[g];
            }
            printf("Trajet direct : %s (de %s à %s) | Temps total : %.1f h\n", trajets[t].nom, depart, arrivee, temps_total);
        }
    }
    // Correspondances (deux trajets, gare commune)
    for (int t1 = 0; t1 < nb_trajets; t1++) {
        int idx_dep = -1;
        for (int g = 0; g < trajets[t1].nb_gares; g++)
            if (strcmp(trajets[t1].gares[g], depart) == 0) idx_dep = g;
        if (idx_dep == -1) continue;
        for (int t2 = 0; t2 < nb_trajets; t2++) {
            if (t1 == t2) continue;
            int idx_arr = -1;
            for (int g = 0; g < trajets[t2].nb_gares; g++)
                if (strcmp(trajets[t2].gares[g], arrivee) == 0) idx_arr = g;
            if (idx_arr == -1) continue;
            // Chercher une gare de correspondance commune (hors départ/arrivée)
            for (int g1 = idx_dep + 1; g1 < trajets[t1].nb_gares - 1; g1++) {
                if (strcmp(trajets[t1].gares[g1], arrivee) == 0) continue;
                int idx_cor2 = -1;
                for (int g2 = 1; g2 < idx_arr; g2++) {
                    if (strcmp(trajets[t2].gares[g2], trajets[t1].gares[g1]) == 0) {
                        idx_cor2 = g2;
                        break;
                    }
                }
                if (idx_cor2 != -1) {
                    float temps1 = 0.0f;
                    for (int g = idx_dep + 1; g <= g1; g++) temps1 += trajets[t1].temps[g];
                    float temps2 = 0.0f;
                    for (int g = idx_cor2 + 1; g <= idx_arr; g++) temps2 += trajets[t2].temps[g];
                    if (temps1 > 0.0f && temps2 > 0.0f) {
                        float temps_total = temps1 + temps2;
                        printf("Correspondance : %s (de %s à %s, %.1f h), puis %s (de %s à %s, %.1f h) | Temps total : %.1f h\n",
                            trajets[t1].nom, depart, trajets[t1].gares[g1], temps1,
                            trajets[t2].nom, trajets[t1].gares[g1], arrivee, temps2,
                            temps_total);
                    }
                }
            }
        }
    }
    pause_for_user();
}

void menu_voyageur() {
    int submenu_choice;
    do {
        printf("=== Menu Voyageur ===\n");
        printf("6. Réserver un trajet par numéro\n");
        printf("7. Modifier une réservation existante\n");
        printf("8. Consulter mes réservations\n");
        printf("9. Modifier mon mot de passe\n");
        printf("10. Retour au menu principal\n");
        printf("Votre choix : ");
        scanf("%d", &submenu_choice);
        getchar();
        switch (submenu_choice) {
            case 6:
                reserver_trajet_par_numero();
                break;
            case 7:
                annuler_reservation();
                break;
            case 8:
                consulter_mes_reservations();
                break;
            case 9:
                changer_mot_de_passe_local();
                pause_for_user();
                break;
            case 10:
                printf("Retour au menu principal...\n");
                break;
            default:
                printf("Choix invalide.\n");
                pause_for_user();
        }
    } while (submenu_choice != 10);
}

// Fonction utilitaire pour nettoyer les espaces en début/fin
void trim(char *str) {
    // Trim début
    while (isspace((unsigned char)*str)) str++;
    // Trim fin
    char *end = str + strlen(str) - 1;
    while (end > str && isspace((unsigned char)*end)) end--;
    *(end+1) = '\0';
}


void display_menu() {
    printf("=== Menu Principal ===\n");
    printf("1. Administrateur\n");
    printf("2. Contrôleur\n");
    printf("3. Voyageur\n");
    printf("8. Quitter\n");
}

void handle_choice(int choice) {
    int submenu_choice;
    switch (choice) {
        case 1:
            if (!authentifier_admin()) break;
            do {
                printf("=== Menu Administrateur ===\n");
                printf("1. Gestion des trajets\n");
                printf("2. Changer le mot de passe\n");
                printf("9. Modifier le mot de passe d'un utilisateur\n");
                printf("8. Retour au menu principal\n");
                printf("Votre choix : ");
                scanf("%d", &submenu_choice);
                getchar(); // Consume newline character

                switch (submenu_choice) {
                    case 1:
                        printf("Gestion des trajets...\n");
                        gestion_trajets();
                        break;
                    case 2:
                        printf("Changer le mot de passe...\n");
                        changer_mot_de_passe_local();
                        break;
                    case 9:
                        changer_mot_de_passe_local();
                        pause_for_user();
                        break;
                    case 8:
                        printf("Retour au menu principal...\n");
                        break;
                    default:
                        printf("Choix invalide.\n");
                        pause_for_user();
                }
            } while (submenu_choice != 8);
            break;
        case 2:
            if (!authentifier_controleur()) break;
            do {
                printf("=== Menu Contrôleur ===\n");
                printf("4. Rechercher un trajet\n");
                printf("5. Changer le mot de passe\n");
                printf("8. Retour au menu principal\n");
                printf("Votre choix : ");
                scanf("%d", &submenu_choice);
                getchar(); // Consume newline character

                switch (submenu_choice) {
                    case 4:
                        printf("Rechercher un trajet...\n");
                        rechercher_trajet_txt_controleur();
                        break;
                    case 5:
                        printf("Changer le mot de passe...\n");
                        changer_mot_de_passe_local();
                        break;
                    case 8:
                        printf("Retour au menu principal...\n");
                        break;
                    default:
                        printf("Choix invalide.\n");
                        pause_for_user();
                }
            } while (submenu_choice != 8);
            break;
        case 3:
            if (!authentifier_user()) break;
            menu_voyageur();
            break;
        case 8:
            printf("Au revoir !\n");
            break;
        default:
            printf("Choix invalide.\n");
            pause_for_user();
    }
}

// Ajout : annulation de réservation
void annuler_reservation() {
    int trouve = 0;
    for (int v = 0; v < voyageurDB.nombre_voyageurs; v++) {
        if (strcmp(voyageurDB.voyageurs[v].identifiant, utilisateur_connecte) == 0) {
            if (voyageurDB.voyageurs[v].nombre_reservations == 0) {
                printf("Vous n'avez aucune réservation à annuler.\n");
                pause_for_user();
                return;
            }
            printf("Vos réservations :\n");
            for (int r = 0; r < voyageurDB.voyageurs[v].nombre_reservations; r++) {
                printf("%d. %s\n", r+1, voyageurDB.voyageurs[v].reservations[r]);
            }
            printf("Entrez le numéro de la réservation à annuler (0 pour annuler) : ");
            int choix = 0;
            scanf("%d", &choix);
            getchar();
            if (choix < 1 || choix > voyageurDB.voyageurs[v].nombre_reservations) {
                printf("Annulation abandonnée.\n");
                pause_for_user();
                return;
            }
            char trajet_nom[50];
            strncpy(trajet_nom, voyageurDB.voyageurs[v].reservations[choix-1], 50);
            for (int t = 0; t < trajetDB.nombre_trajets; t++) {
                if (strcmp(trajetDB.trajets[t].nom, trajet_nom) == 0) {
                    trajetDB.trajets[t].places_libres++;
                    exporter_trajets_txt(); // MAJ du fichier texte après annulation
                    break;
                }
            }
            for (int r = choix-1; r < voyageurDB.voyageurs[v].nombre_reservations-1; r++) {
                strncpy(voyageurDB.voyageurs[v].reservations[r], voyageurDB.voyageurs[v].reservations[r+1], 50);
            }
            voyageurDB.voyageurs[v].nombre_reservations--;
            printf("Réservation annulée.\n");
            exporter_json("data/exported_data.json");
            trouve = 1;
            break;
        }
    }
    if (!trouve) {
        printf("Erreur : utilisateur non trouvé.\n");
    }
    pause_for_user();
}

// Affiche la liste des trajets (directs ou avec correspondance) entre deux gares, puis permet la réservation par numéro
void reserver_trajet_par_numero() {
    char depart[50], arrivee[50];
    printf("Gare de départ : ");
    scanf("%49s", depart);
    printf("Gare d'arrivée : ");
    scanf("%49s", arrivee);
    getchar();

    typedef struct {
        int t1, t2; // index des trajets dans le tableau trajets[] (t2 == -1 si direct)
        int g_dep, g_cor, g_arr; // indices gares dans le trajet
        float temps1, temps2, temps_total;
        char cor[MAX_NOM_TXT]; // nom de la gare de correspondance (si t2 != -1)
    } OptionTxt;
    TrajetSimple trajets[MAX_TRAJETS_TXT];
    int nb_trajets = 0;
    OptionTxt options[MAX_TRAJETS_TXT * MAX_TRAJETS_TXT];
    int nb_options = 0;
    FILE *f = fopen("trajet.txt", "r");
    if (!f) {
        printf("Impossible d'ouvrir trajet.txt\n");
        pause_for_user();
        return;
    }
    char ligne[512];
    while (fgets(ligne, sizeof(ligne), f) && nb_trajets < MAX_TRAJETS_TXT) {
        char *token = strtok(ligne, " \n");
        int g = 0;
        if (token) {
            strncpy(trajets[nb_trajets].nom, token, MAX_NOM_TXT);
            token = strtok(NULL, " \n");
        } else {
            trajets[nb_trajets].nom[0] = '\0';
        }
        while (token && g < MAX_GARES_TXT) {
            strncpy(trajets[nb_trajets].gares[g], token, MAX_NOM_TXT);
            token = strtok(NULL, " \n");
            if (token) {
                trajets[nb_trajets].temps[g] = atof(token);
                token = strtok(NULL, " \n");
            } else {
                trajets[nb_trajets].temps[g] = 0.0f;
            }
            g++;
        }
        trajets[nb_trajets].nb_gares = g;
        nb_trajets++;
    }
    fclose(f);
    // Recherche trajets directs
    for (int t = 0; t < nb_trajets; t++) {
        int idx_dep = -1, idx_arr = -1;
        for (int g = 0; g < trajets[t].nb_gares; g++) {
            if (strcmp(trajets[t].gares[g], depart) == 0) idx_dep = g;
            if (strcmp(trajets[t].gares[g], arrivee) == 0) idx_arr = g;
        }
        if (idx_dep != -1 && idx_arr != -1 && idx_dep < idx_arr) {
            float temps_total = 0.0f;
            for (int g = idx_dep + 1; g <= idx_arr; g++) temps_total += trajets[t].temps[g];
            options[nb_options++] = (OptionTxt){t, -1, idx_dep, -1, idx_arr, temps_total, 0.0f, temps_total, ""};
        }
    }
    // Recherche correspondances (2 trajets, gare commune)
    for (int t1 = 0; t1 < nb_trajets; t1++) {
        int idx_dep = -1;
        for (int g = 0; g < trajets[t1].nb_gares; g++)
            if (strcmp(trajets[t1].gares[g], depart) == 0) idx_dep = g;
        if (idx_dep == -1) continue;
        for (int g1 = idx_dep + 1; g1 < trajets[t1].nb_gares - 1; g1++) {
            char *correspondance = trajets[t1].gares[g1];
            if (strcmp(correspondance, arrivee) == 0) continue;
            for (int t2 = 0; t2 < nb_trajets; t2++) {
                if (t1 == t2) continue;
                int idx_cor = -1, idx_arr = -1;
                for (int g2 = 0; g2 < trajets[t2].nb_gares; g2++) {
                    if (strcmp(trajets[t2].gares[g2], correspondance) == 0) idx_cor = g2;
                    if (strcmp(trajets[t2].gares[g2], arrivee) == 0) idx_arr = g2;
                }
                if (idx_cor != -1 && idx_arr != -1 && idx_cor < idx_arr) {
                    float temps1 = 0.0f, temps2 = 0.0f;
                    for (int g = idx_dep + 1; g <= g1; g++) temps1 += trajets[t1].temps[g];
                    for (int g = idx_cor + 1; g <= idx_arr; g++) temps2 += trajets[t2].temps[g];
                    if (temps1 > 0.0f && temps2 > 0.0f) {
                        options[nb_options++] = (OptionTxt){t1, t2, idx_dep, g1, idx_arr, temps1, temps2, temps1 + temps2, ""};
                        strncpy(options[nb_options-1].cor, correspondance, MAX_NOM_TXT);
                    }
                }
            }
        }
    }
    if (nb_options == 0) {
        printf("Aucun trajet direct ou avec correspondance trouvé entre %s et %s.\n", depart, arrivee);
        pause_for_user();
        return;
    }
    printf("\nListe des trajets disponibles :\n");
    for (int o = 0; o < nb_options; o++) {
        if (options[o].t2 == -1) {
            // Direct
            int idx_db = -1;
            for (int i = 0; i < trajetDB.nombre_trajets; i++)
                if (strcmp(trajetDB.trajets[i].nom, trajets[options[o].t1].nom) == 0) idx_db = i;
            int places = (idx_db != -1) ? trajetDB.trajets[idx_db].places_libres : 0;
            printf("%d. Direct : %s (temps : %.1f h, places libres : %d)\n", o+1, trajets[options[o].t1].nom, options[o].temps_total, places);
        } else {
            int idx_db1 = -1, idx_db2 = -1;
            for (int i = 0; i < trajetDB.nombre_trajets; i++) {
                if (strcmp(trajetDB.trajets[i].nom, trajets[options[o].t1].nom) == 0) idx_db1 = i;
                if (strcmp(trajetDB.trajets[i].nom, trajets[options[o].t2].nom) == 0) idx_db2 = i;
            }
            int places1 = (idx_db1 != -1) ? trajetDB.trajets[idx_db1].places_libres : 0;
            int places2 = (idx_db2 != -1) ? trajetDB.trajets[idx_db2].places_libres : 0;
            printf("%d. Correspondance : %s (de %s à %s, %.1f h, places : %d), puis %s (de %s à %s, %.1f h, places : %d) | Temps total : %.1f h\n",
                o+1,
                trajets[options[o].t1].nom, depart, options[o].cor, options[o].temps1, places1,
                trajets[options[o].t2].nom, options[o].cor, arrivee, options[o].temps2, places2,
                options[o].temps_total);
        }
    }
    printf("0. Quitter\n");
    printf("Entrez le numéro du trajet à réserver (0 pour quitter) : ");
    int choix = 0;
    scanf("%d", &choix);
    getchar();
    if (choix == 0) {
        printf("Annulation de la réservation.\n");
        pause_for_user();
        return;
    }
    if (choix < 1 || choix > nb_options) {
        printf("Numéro invalide.\n");
        pause_for_user();
        return;
    }
    OptionTxt sel = options[choix-1];
    // Réservation effective dans trajetDB
    int v_idx = -1;
    for (int v = 0; v < voyageurDB.nombre_voyageurs; v++) {
        if (strcmp(voyageurDB.voyageurs[v].identifiant, utilisateur_connecte) == 0) {
            v_idx = v;
            break;
        }
    }
    if (v_idx == -1) {
        printf("Erreur : utilisateur non trouvé.\n");
        pause_for_user();
        return;
    }

    if (sel.t2 == -1) {
        // Direct
        int idx_db = -1;
        for (int i = 0; i < trajetDB.nombre_trajets; i++)
            if (strcmp(trajetDB.trajets[i].nom, trajets[sel.t1].nom) == 0) idx_db = i;
        if (idx_db == -1) {
            printf("Erreur : trajet introuvable pour réservation.\n");
            pause_for_user();
            return;
        }
        if (trajetDB.trajets[idx_db].places_libres > 0) {
            trajetDB.trajets[idx_db].places_libres--;
            strncpy(voyageurDB.voyageurs[v_idx].reservations[voyageurDB.voyageurs[v_idx].nombre_reservations], trajetDB.trajets[idx_db].nom, 50);
            voyageurDB.voyageurs[v_idx].nombre_reservations++;
            printf("Réservation effectuée sur '%s'. Places restantes : %d\n", trajetDB.trajets[idx_db].nom, trajetDB.trajets[idx_db].places_libres);
            exporter_json("data/exported_data.json");
        } else {
            printf("Aucune place disponible sur ce trajet.\n");
        }
    } else {
        // Correspondance
        int idx_db1 = -1, idx_db2 = -1;
        for (int i = 0; i < trajetDB.nombre_trajets; i++) {
            if (strcmp(trajetDB.trajets[i].nom, trajets[sel.t1].nom) == 0) idx_db1 = i;
            if (strcmp(trajetDB.trajets[i].nom, trajets[sel.t2].nom) == 0) idx_db2 = i;
        }
        if (idx_db1 == -1 || idx_db2 == -1) {
            printf("Erreur : un des trajets de la correspondance est introuvable.\n");
            pause_for_user();
            return;
        }
        if (trajetDB.trajets[idx_db1].places_libres > 0 && trajetDB.trajets[idx_db2].places_libres > 0) {
            trajetDB.trajets[idx_db1].places_libres--;
            trajetDB.trajets[idx_db2].places_libres--;
            strncpy(voyageurDB.voyageurs[v_idx].reservations[voyageurDB.voyageurs[v_idx].nombre_reservations], trajetDB.trajets[idx_db1].nom, 50);
            voyageurDB.voyageurs[v_idx].nombre_reservations++;
            strncpy(voyageurDB.voyageurs[v_idx].reservations[voyageurDB.voyageurs[v_idx].nombre_reservations], trajetDB.trajets[idx_db2].nom, 50);
            voyageurDB.voyageurs[v_idx].nombre_reservations++;
            printf("Réservation effectuée sur '%s' et '%s'. Places restantes : %d, %d\n", trajetDB.trajets[idx_db1].nom, trajetDB.trajets[idx_db2].nom, trajetDB.trajets[idx_db1].places_libres, trajetDB.trajets[idx_db2].places_libres);
            exporter_json("data/exported_data.json");
        } else {
            printf("Aucune place disponible sur l'un des trajets de la correspondance.\n");
        }
    }
    pause_for_user();
}