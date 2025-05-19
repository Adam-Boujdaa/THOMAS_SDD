#include "trajet.h"
#include "voyageur.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <cjson/cJSON.h>

TrajetDB trajetDB = { .nombre_trajets = 0 };
extern VoyageurDB voyageurDB;

void afficher_trajets();

// Fonction pour initialiser un trajet avec un identifiant donné
Trajet trajet_init(int id) {
    Trajet trajet;
    snprintf(trajet.nom, sizeof(trajet.nom), "Trajet %d", id);
    trajet.nombre_gares = 0;
    return trajet;
}

// Fonction pour ajouter une gare à un trajet avec son nom et son temps
void trajet_ajouter_gare(Trajet *trajet, const char *nom, float temps) {
    if (trajet->nombre_gares < MAX_GARES) {
        strncpy(trajet->gares[trajet->nombre_gares].nom, nom, sizeof(trajet->gares[trajet->nombre_gares].nom));
        trajet->gares[trajet->nombre_gares].temps = temps;
        trajet->nombre_gares++;
        printf("[DEBUG] trajet_ajouter_gare: Ajout de la gare '%s' (%.1f) au trajet '%s' (total: %d)\n", nom, temps, trajet->nom, trajet->nombre_gares);
    } else {
        printf("[ERREUR] trajet_ajouter_gare: Limite de gares atteinte (%d) pour le trajet '%s'\n", MAX_GARES, trajet->nom);
    }
}

// Fonction pour afficher les informations d'un trajet
void trajet_afficher(const Trajet *trajet) {
    printf("Trajet: %s\n", trajet->nom);
    for (int i = 0; i < trajet->nombre_gares; i++) {
        printf("  Gare: %s, Temps: %.1f heures\n", trajet->gares[i].nom, trajet->gares[i].temps);
    }
}

// Fonction pour fusionner deux trajets en un seul
Trajet trajet_fusionner(const Trajet *trajet1, const Trajet *trajet2) {
    Trajet fusionne = trajet_init(0);
    snprintf(fusionne.nom, sizeof(fusionne.nom), "Fusion"); // Simplifié pour éviter la troncature
    for (int i = 0; i < trajet1->nombre_gares; i++) {
        trajet_ajouter_gare(&fusionne, trajet1->gares[i].nom, trajet1->gares[i].temps);
    }
    for (int i = 0; i < trajet2->nombre_gares; i++) {
        trajet_ajouter_gare(&fusionne, trajet2->gares[i].nom, trajet2->gares[i].temps);
    }
    return fusionne;
}

// Fonction pour prolonger un trajet en ajoutant une nouvelle gare
void trajet_prolonger(Trajet *trajet, const char *nom, float temps) {
    trajet_ajouter_gare(trajet, nom, temps);
}

void trajet_raccourcir(Trajet *trajet, const char *nom) {
    for (int i = 0; i < trajet->nombre_gares; i++) {
        if (strcmp(trajet->gares[i].nom, nom) == 0) {
            trajet->nombre_gares = i + 1;
            break;
        }
    }
}

void trajet_sauvegarder(const Trajet *trajet, const char *filename) {
    FILE *file = fopen(filename, "w");
    if (file) {
        fprintf(file, "%s\n", trajet->nom);
        fprintf(file, "%d\n", trajet->nombre_gares);
        for (int i = 0; i < trajet->nombre_gares; i++) {
            fprintf(file, "%s %.1f\n", trajet->gares[i].nom, trajet->gares[i].temps);
        }
        fclose(file);
    }
}

Trajet trajet_charger(const char *filename) {
    Trajet trajet = trajet_init(0);
    FILE *file = fopen(filename, "r");
    if (file) {
        fgets(trajet.nom, sizeof(trajet.nom), file);
        trajet.nom[strcspn(trajet.nom, "\n")] = '\0';
        fscanf(file, "%d\n", &trajet.nombre_gares);
        for (int i = 0; i < trajet.nombre_gares; i++) {
            fscanf(file, "%s %f\n", trajet.gares[i].nom, &trajet.gares[i].temps);
        }
        fclose(file);
    }
    return trajet;
}

void trajet_effacer(Trajet *trajet) {
    trajet->nombre_gares = 0;
}

void load_trajets_from_file(const char *filename, Trajet trajets[], int *trajet_count) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("Erreur : Impossible d'ouvrir le fichier %s\n", filename);
        return;
    }

    fscanf(file, "%d\n", trajet_count);
    for (int i = 0; i < *trajet_count; i++) {
        fgets(trajets[i].nom, sizeof(trajets[i].nom), file);
        trajets[i].nom[strcspn(trajets[i].nom, "\n")] = '\0';
        fscanf(file, "%d\n", &trajets[i].nombre_gares);
        for (int j = 0; j < trajets[i].nombre_gares; j++) {
            fscanf(file, "%s %f\n", trajets[i].gares[j].nom, &trajets[i].gares[j].temps);
        }
    }

    fclose(file);
}

Trajet trajet_rechercher(const char *gare_depart, const char *gare_arrivee) {
    for (int i = 0; i < trajetDB.nombre_trajets; i++) {
        Trajet *trajet = &trajetDB.trajets[i];
        if (strcmp(trajet->gares[0].nom, gare_depart) == 0 &&
            strcmp(trajet->gares[trajet->nombre_gares - 1].nom, gare_arrivee) == 0) {
            return *trajet;
        }
    }
    Trajet empty_trajet = trajet_init(0); // Return an empty trajet if not found
    return empty_trajet;
}

void ajouter_trajet(const char *nom, const char *gare_depart, const char *gare_arrivee) {
    if (trajetDB.nombre_trajets < MAX_TRAJETS) {
        Trajet *new_trajet = &trajetDB.trajets[trajetDB.nombre_trajets++];
        strncpy(new_trajet->nom, nom, sizeof(new_trajet->nom));
        trajet_ajouter_gare(new_trajet, gare_depart, 0.0);
        trajet_ajouter_gare(new_trajet, gare_arrivee, 1.0); // Example duration
        printf("Trajet ajouté avec succès.\n");
    } else {
        printf("Erreur : Limite de trajets atteinte.\n");
    }
}

void modifier_trajet(const char *nom, const char *nouvelle_gare_depart, const char *nouvelle_gare_arrivee) {
    for (int i = 0; i < trajetDB.nombre_trajets; i++) {
        Trajet *trajet = &trajetDB.trajets[i];
        if (strcmp(trajet->nom, nom) == 0) {
            trajet_effacer(trajet);
            trajet_ajouter_gare(trajet, nouvelle_gare_depart, 0.0);
            trajet_ajouter_gare(trajet, nouvelle_gare_arrivee, 1.0); // Example duration
            printf("Trajet modifié avec succès.\n");
            return;
        }
    }
    printf("Erreur : Trajet non trouvé.\n");
}

void supprimer_trajet(const char *nom) {
    for (int i = 0; i < trajetDB.nombre_trajets; i++) {
        if (strcmp(trajetDB.trajets[i].nom, nom) == 0) {
            for (int j = i; j < trajetDB.nombre_trajets - 1; j++) {
                trajetDB.trajets[j] = trajetDB.trajets[j + 1];
            }
            trajetDB.nombre_trajets--;
            printf("Trajet supprimé avec succès.\n");
            return;
        }
    }
    printf("Erreur : Trajet non trouvé.\n");
}

void afficher_menu_principal() {
    printf("=== Menu Principal ===\n");
    printf("1. Administrateur\n");
    printf("2. Contrôleur\n");
    printf("3. Voyageur\n");
    printf("8. Quitter\n");
    printf("Votre choix : ");
    fflush(stdout); // Assure que le texte est affiché immédiatement
}

void afficher_menu_administrateur() {
    printf("=== Menu Administrateur ===\n");
    printf("1. Gestion des trajets\n");
    printf("2. Exportation des trajets et voyageurs au format JSON\n");
    printf("3. Changer le mot de passe\n");
    printf("8. Retour au menu principal\n");
    printf("Votre choix : ");
    fflush(stdout);
}

void afficher_menu_gestion_trajets() {
    printf("=== Gestion des trajets ===\n");
    printf("1. Ajouter un trajet\n");
    printf("2. Modifier un trajet\n");
    printf("3. Supprimer un trajet\n");
    printf("4. Retour au menu administrateur\n");
    printf("Votre choix : ");
    fflush(stdout);
}

void ajouter_trajet_interactif() {
    char nom[50];
    char gare_depart[50];
    char gare_arrivee[50];
    float temps;

    printf("Ajout d'un trajet...\n");
    printf("Entrez le nom du trajet : ");
    fgets(nom, sizeof(nom), stdin);
    nom[strcspn(nom, "\n")] = '\0'; // Supprime le \n

    printf("Entrez le nom de la gare de départ : ");
    fgets(gare_depart, sizeof(gare_depart), stdin);
    gare_depart[strcspn(gare_depart, "\n")] = '\0';

    printf("Entrez le nom de la gare d'arrivée : ");
    fgets(gare_arrivee, sizeof(gare_arrivee), stdin);
    gare_arrivee[strcspn(gare_arrivee, "\n")] = '\0';

    printf("Entrez le temps de trajet (en heures) : ");
    scanf("%f", &temps);
    getchar(); // Consomme le \n restant

    Trajet trajet = trajet_init(trajetDB.nombre_trajets + 1);
    strncpy(trajet.nom, nom, sizeof(trajet.nom));
    trajet_ajouter_gare(&trajet, gare_depart, 0.0);
    trajet_ajouter_gare(&trajet, gare_arrivee, temps);

    trajetDB.trajets[trajetDB.nombre_trajets++] = trajet;
    printf("Trajet ajouté avec succès.\n");
}

void modifier_trajet_interactif() {
    char nom[50];
    printf("Modification d'un trajet...\n");
    afficher_trajets();
    printf("Entrez le nom du trajet à modifier : ");
    fgets(nom, sizeof(nom), stdin);
    nom[strcspn(nom, "\n")] = '\0';
    for (int i = 0; i < trajetDB.nombre_trajets; i++) {
        if (strcmp(trajetDB.trajets[i].nom, nom) == 0) {
            printf("Trajet trouvé : %s\n", trajetDB.trajets[i].nom);
            printf("Combien de gares pour ce trajet (min 2) ? ");
            int nb_gares = 0;
            scanf("%d", &nb_gares);
            getchar();
            if (nb_gares < 2 || nb_gares > MAX_GARES) {
                printf("Nombre de gares invalide.\n");
                return;
            }
            trajet_effacer(&trajetDB.trajets[i]);
            strncpy(trajetDB.trajets[i].nom, nom, sizeof(trajetDB.trajets[i].nom));
            for (int g = 0; g < nb_gares; g++) {
                char gare_nom[50];
                float temps = 0.0f;
                printf("Nom de la gare %d : ", g+1);
                fgets(gare_nom, sizeof(gare_nom), stdin);
                gare_nom[strcspn(gare_nom, "\n")] = '\0';
                if (g == 0) {
                    trajet_ajouter_gare(&trajetDB.trajets[i], gare_nom, 0.0f);
                } else {
                    printf("Temps pour atteindre cette gare (en heures) : ");
                    scanf("%f", &temps);
                    getchar();
                    trajet_ajouter_gare(&trajetDB.trajets[i], gare_nom, temps);
                }
            }
            printf("Trajet modifié avec succès.\n");
            exporter_trajets_txt();
            return;
        }
    }
    printf("Erreur : Trajet non trouvé.\n");
}

void supprimer_trajet_interactif() {
    char nom[50];

    printf("Suppression d'un trajet...\n");
    printf("Entrez le nom du trajet à supprimer : ");
    fgets(nom, sizeof(nom), stdin);
    nom[strcspn(nom, "\n")] = '\0';

    for (int i = 0; i < trajetDB.nombre_trajets; i++) {
        if (strcmp(trajetDB.trajets[i].nom, nom) == 0) {
            for (int j = i; j < trajetDB.nombre_trajets - 1; j++) {
                trajetDB.trajets[j] = trajetDB.trajets[j + 1];
            }
            trajetDB.nombre_trajets--;
            printf("Trajet supprimé avec succès.\n");
            return;
        }
    }
    printf("Erreur : Trajet non trouvé.\n");
}

void importer_trajets_depuis_json(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("Erreur : Impossible d'ouvrir le fichier %s\n", filename);
        return;
    }
    fseek(file, 0, SEEK_END);
    long length = ftell(file);
    fseek(file, 0, SEEK_SET);
    char *data = malloc(length + 1);
    fread(data, 1, length, file);
    data[length] = '\0';
    fclose(file);

    cJSON *root = cJSON_Parse(data);
    if (!root) {
        printf("Erreur : JSON invalide dans %s\n", filename);
        free(data);
        return;
    }
    cJSON *trajets = cJSON_GetObjectItem(root, "trajets");
    if (!cJSON_IsArray(trajets)) {
        printf("Erreur : 'trajets' n'est pas un tableau dans %s\n", filename);
        cJSON_Delete(root);
        free(data);
        return;
    }
    trajetDB.nombre_trajets = 0;
    cJSON *trajet;
    cJSON_ArrayForEach(trajet, trajets) {
        if (trajetDB.nombre_trajets >= MAX_TRAJETS) break;
        Trajet *t = &trajetDB.trajets[trajetDB.nombre_trajets++];
        memset(t, 0, sizeof(Trajet)); // Initialisation mémoire à zéro
        cJSON *nom = cJSON_GetObjectItem(trajet, "nom");
        cJSON *gares = cJSON_GetObjectItem(trajet, "gares");
        if (!cJSON_IsString(nom) || !cJSON_IsArray(gares)) continue;
        strncpy(t->nom, nom->valuestring, sizeof(t->nom));
        t->nombre_gares = 0;
        cJSON *gare;
        cJSON_ArrayForEach(gare, gares) {
            if (t->nombre_gares >= MAX_GARES) break;
            cJSON *nom_gare = cJSON_GetObjectItem(gare, "nom");
            cJSON *temps = cJSON_GetObjectItem(gare, "temps");
            if (!cJSON_IsString(nom_gare) || !cJSON_IsNumber(temps)) continue;
            strncpy(t->gares[t->nombre_gares].nom, nom_gare->valuestring, sizeof(t->gares[t->nombre_gares].nom));
            t->gares[t->nombre_gares].temps = (float)temps->valuedouble;
            t->nombre_gares++;
        }
        // Toujours forcer à 10 lors de l'import JSON
        t->capacite = 10;
        t->places_libres = 10;
    }

    // Après l'import des trajets, importer les réservations utilisateurs
    // cJSON *utilisateurs = cJSON_GetObjectItem(root, "utilisateurs");
    // if (cJSON_IsArray(utilisateurs)) {
    //     voyageurDB.nombre_voyageurs = 0;
    //     cJSON *utilisateur;
    //     cJSON_ArrayForEach(utilisateur, utilisateurs) {
    //         if (voyageurDB.nombre_voyageurs >= MAX_VOYAGEURS) break;
    //         cJSON *nom_utilisateur = cJSON_GetObjectItem(utilisateur, "nom_utilisateur");
    //         if (!cJSON_IsString(nom_utilisateur)) continue;
    //         Voyageur *v = &voyageurDB.voyageurs[voyageurDB.nombre_voyageurs++];
    //         memset(v, 0, sizeof(Voyageur));
    //         strncpy(v->nom, nom_utilisateur->valuestring, sizeof(v->nom));
    //         cJSON *reservations = cJSON_GetObjectItem(utilisateur, "reservations");
    //         v->nombre_reservations = 0;
    //         if (cJSON_IsArray(reservations)) {
    //             cJSON *reservation;
    //             cJSON_ArrayForEach(reservation, reservations) {
    //                 if (v->nombre_reservations >= MAX_TRAJETS) break;
    //                 if (cJSON_IsString(reservation)) {
    //                     strncpy(v->reservations[v->nombre_reservations], reservation->valuestring, 50);
    //                     v->nombre_reservations++;
    //                 }
    //             }
    //         }
    //     }
    // }

    cJSON_Delete(root);
    free(data);
}