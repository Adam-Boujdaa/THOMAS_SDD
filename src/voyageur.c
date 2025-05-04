#include "voyageur.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


struct s_voyageur {
    char nom[MAX_NOM];
    int id;
    int trajets_reserves[MAX_TRAJETS];
    int nb_trajets;
};

Voyageur creer_voyageur(const char* nom, int id) {
    Voyageur v = (Voyageur)malloc(sizeof(struct s_voyageur));
    if (!v) {
        fprintf(stderr, "Erreur d'allocation mémoire pour le voyageur.\n");
        exit(EXIT_FAILURE);
    }

    strncpy(v->nom, nom, MAX_NOM);
    v->nom[MAX_NOM - 1] = '\0';
    v->id = id;
    v->nb_trajets = 0;

    for (int i = 0; i < MAX_TRAJETS; i++) {
        v->trajets_reserves[i] = -1; // Aucun trajet
    }

    return v;
}


void reserver_trajet(Voyageur voyageur, int id_trajet) {
    if (!voyageur) {
        printf("Ce voyageur ne figure pas dans notre base de données.");
        return;
    }

    if (voyageur->nb_trajets >= MAX_TRAJETS) {
        printf("Limite de trajets atteinte pour ce voyageur.\n");
        return;
    }

    if (trajet_existe(voyageur, id_trajet)) {
        printf("Trajet déjà réservé par ce voyageur.\n");
        return;
    }

    voyageur->trajets_reserves[voyageur->nb_trajets++] = id_trajet;
    printf("Trajet %d réservé avec succès.\n", id_trajet);
}

void modifier_trajet(Voyageur voyageur, int ancien_trajet, int nouveau_trajet) {
    if (!voyageur) return;

    for (int i = 0; i < voyageur->nb_trajets; i++) {
        if (voyageur->trajets_reserves[i] == ancien_trajet) {
            voyageur->trajets_reserves[i] = nouveau_trajet;
            printf("Trajet modifié : %d -> %d\n", ancien_trajet, nouveau_trajet);
            return;
        }
    }

    printf("Ancien trajet non trouvé pour ce voyageur.\n");
}

int trajet_existe(Voyageur voyageur, int id_trajet) {
    if (!voyageur) return 0;

    for (int i = 0; i < voyageur->nb_trajets; i++) {
        if (voyageur->trajets_reserves[i] == id_trajet) {
            return 1;
        }
    }
    return 0;
}


void afficher_voyageur(Voyageur voyageur) {
    if (!voyageur) return;

    printf("----- Infos Voyageur -----\n");
    printf("Nom: %s\n", voyageur->nom);
    printf("ID: %d\n", voyageur->id);
    printf("Trajets réservés (%d): ", voyageur->nb_trajets);

    for (int i = 0; i < voyageur->nb_trajets; i++) {
        printf("%d ", voyageur->trajets_reserves[i]);
    }
    printf("\n");
}

void liberer_voyageur(Voyageur voyageur) {
    if (voyageur) {
        free(voyageur);
    }
}
