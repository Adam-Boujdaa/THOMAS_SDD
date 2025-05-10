#include "voyageur.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_RESERVATIONS 50
#define TAILLE_NOM 100

struct s_voyageur {
    char nom[TAILLE_NOM];
    int identifiant;
    Trajet* trajets_reserves;
    int nombre_reservations;
};

// Création d'un voyageur
Voyageur voyageur_init(const char* nom, int identifiant) {
    Voyageur v = malloc(sizeof(struct s_voyageur));
    if (!v) return NULL;
    strncpy(v->nom, nom, TAILLE_NOM-1);
    v->nom[TAILLE_NOM-1] = '\0';
    v->identifiant = identifiant;
    v->trajets_reserves = malloc(MAX_RESERVATIONS * sizeof(Trajet));
    v->nombre_reservations = 0;
    return v;
}

// Destruction d'un voyageur
void voyageur_effacer(Voyageur* voyageur) {
    if (*voyageur) {
        free((*voyageur)->trajets_reserves);
        free(*voyageur);
        *voyageur = NULL;
    }
}

// Recherche d'un voyageur par identifiant
Voyageur voyageur_rechercher_par_id(int identifiant, Voyageur* liste, int taille) {
    for (int i = 0; i < taille; i++) {
        if (liste[i]->identifiant == identifiant) {
            return liste[i];
        }
    }
    return NULL;
}

// Réservation de trajet
int voyageur_reserver_trajet(Voyageur voyageur, Trajet trajet, Place* places, int nb_places) {
    if (voyageur->nombre_reservations >= MAX_RESERVATIONS) return 0;
    if (trajet_places_disponibles(trajet, places, nb_places) > 0) {
        voyageur->trajets_reserves[voyageur->nombre_reservations++] = trajet;
        return 1;
    }
    return 0;
}

// Modification de trajet
int voyageur_modifier_trajet(Voyageur voyageur, Trajet ancien, Trajet nouveau, Place* places, int nb_places) {
    for (int i = 0; i < voyageur->nombre_reservations; i++) {
        if (voyageur->trajets_reserves[i] == ancien) {
            if (trajet_places_disponibles(nouveau, places, nb_places) > 0) {
                voyageur->trajets_reserves[i] = nouveau;
                return 1;
            }
        }
    }
    return 0;
}

// verif du nombre de places disponibles pour un trajet
int trajet_places_disponibles(Trajet trajet, Place* places, int nb_places) {
    int compteur = 0;
    for (int i = 0; i < nb_places; i++) {
        if (place_get_trajet(places[i]) == trajet && place_est_libre(places[i])) {
            compteur++;
        }
    }
    return compteur;
}

// Menu voyageur (exemple, à adapter à tes besoins)
void menu_voyageur(Voyageur* liste_voyageurs, int nb_voyageurs, Trajet* liste_trajets, int nb_trajets, Place* places, int nb_places) {
    int choix, identifiant, id_trajet, i;
    char depart[50], arrivee[50];

    do {
        printf("\n=== Menu Voyageur ===\n");
        printf("1. Rechercher par gares\n");
        printf("2. Réserver un trajet\n");
        printf("3. Modifier une réservation\n");
        printf("4. Quitter\n");
        scanf("%d", &choix);

        if (choix == 1) {
            printf("Gare de départ : ");
            scanf("%s", depart);
            printf("Gare d'arrivée : ");
            scanf("%s", arrivee);
            for (i = 0; i < nb_trajets; i++) {
                const char* gare_depart = trajet_get_depart(liste_trajets[i]);
                const char* gare_arrivee = trajet_get_arrivee(liste_trajets[i]);
                if (strcmp(gare_depart, depart) == 0 && strcmp(gare_arrivee, arrivee) == 0) {
                    printf("Trajet trouvé : %d\n", trajet_get_id(liste_trajets[i]));
                }
            }
        } else if (choix == 2) {
            printf("ID voyageur : ");
            scanf("%d", &identifiant);
            Voyageur v = voyageur_rechercher_par_id(identifiant, liste_voyageurs, nb_voyageurs);
            if (v) {
                printf("ID trajet à réserver : ");
                scanf("%d", &id_trajet);
                Trajet trajet_trouve = NULL;
                for (i = 0; i < nb_trajets; i++) {
                    if (trajet_get_id(liste_trajets[i]) == id_trajet) {
                        trajet_trouve = liste_trajets[i];
                        break;
                    }
                }
                if (trajet_trouve) {
                    if (voyageur_reserver_trajet(v, trajet_trouve, places, nb_places)) {
                        printf("Réservation réussie !\n");
                    } else {
                        printf("Réservation impossible (plus de places ou max atteint).\n");
                    }
                } else {
                    printf("Trajet non trouvé.\n");
                }
            } else {
                printf("Voyageur non trouvé.\n");
            }
        }
        // À compléter pour les autres choix
    } while (choix != 4);
}
