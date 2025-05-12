
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "voyageur.h"

struct s_voyageur {
    char nom[TAILLE_NOM];
    int identifiant;
    Trajet trajets_reserves[MAX_RESERVATIONS];
    int nombre_reservations;
};

Voyageur voyageur_init(const char* nom, int identifiant) {
    Voyageur v = malloc(sizeof(struct s_voyageur));
    if (!v) return NULL;
    strncpy(v->nom, nom, TAILLE_NOM - 1);
    v->nom[TAILLE_NOM - 1] = '\0';
    v->identifiant = identifiant;
    v->nombre_reservations = 0;
    return v;
}

void voyageur_effacer(Voyageur* voyageur) {
    if (*voyageur) {
        free(*voyageur);
        *voyageur = NULL;
    }
}

int voyageur_get_identifiant(Voyageur v) {
    return v->identifiant;
}

const char* voyageur_get_nom(Voyageur v) {
    return v->nom;
}

Voyageur voyageur_rechercher_par_id(int identifiant, Voyageur* liste, int taille) {
    for (int i = 0; i < taille; i++) {
        if (liste[i] && liste[i]->identifiant == identifiant) {
            return liste[i];
        }
    }
    return NULL;
}

int voyageur_reserver_trajet(Voyageur voyageur, Trajet trajet) {
    if (!voyageur || !trajet || voyageur->nombre_reservations >= MAX_RESERVATIONS) return 0;
    if (trajet_places_libres(trajet) <= 0) return 0;

    int num_place = trajet_reserver_place(trajet);
    if (num_place == -1) return 0;

    voyageur->trajets_reserves[voyageur->nombre_reservations++] = trajet;
    return 1;
}

int voyageur_modifier_trajet(Voyageur voyageur, Trajet ancien, Trajet nouveau) {
    for (int i = 0; i < voyageur->nombre_reservations; i++) {
        if (voyageur->trajets_reserves[i] == ancien) {
            if (trajet_places_libres(nouveau) > 0) {
                trajet_annuler_place(ancien, 0); // Numéro de place à gérer correctement
                trajet_reserver_place(nouveau);
                voyageur->trajets_reserves[i] = nouveau;
                return 1;
            }
        }
    }
    return 0;
}

void voyageur_afficher_reservations(Voyageur voyageur) {
    printf("\n--- Réservations de %s ---\n", voyageur->nom);
    if (voyageur->nombre_reservations == 0) {
        printf("Aucune réservation.\n");
    } else {
        for (int i = 0; i < voyageur->nombre_reservations; i++) {
            printf("Réservation %d :\n", i + 1);
            trajet_afficher(voyageur->trajets_reserves[i]);
        }
    }
}

void menu_principal(Voyageur* liste_voyageurs, int* nb_voyageurs, Trajet* liste_trajets, int nb_trajets) {
    int choix;
    Voyageur utilisateur_connecte = NULL;

    do {
        printf("\n=== MENU PRINCIPAL ===\n");
        printf("1. Se connecter\n");
        printf("2. Créer un compte\n");
        printf("3. Quitter\n");
        printf("Choix : ");
        scanf("%d", &choix);

        if (choix == 1) {
            int id;
            printf("Entrez votre identifiant : ");
            scanf("%d", &id);
            utilisateur_connecte = voyageur_rechercher_par_id(id, liste_voyageurs, *nb_voyageurs);
            if (utilisateur_connecte) {
                printf("Connexion réussie. Bonjour %s !\n", utilisateur_connecte->nom);
                int choix_utilisateur;
                do {
                    printf("\n=== MENU VOYAGEUR ===\n");
                    printf("1. Réserver un trajet\n");
                    printf("2. Modifier une réservation\n");
                    printf("3. Voir mes réservations\n");
                    printf("4. Supprimer mon compte\n");
                    printf("5. Se déconnecter\n");
                    printf("Choix : ");
                    scanf("%d", &choix_utilisateur);

                    if (choix_utilisateur == 1) {
                        printf("Liste des trajets disponibles :\n");
                        for (int i = 0; i < nb_trajets; i++) {
                            printf("ID: %d\n", trajet_get_id(liste_trajets[i]));
                            trajet_afficher(liste_trajets[i]);
                        }
                        int id_trajet;
                        printf("Entrez l'ID du trajet à réserver : ");
                        scanf("%d", &id_trajet);
                        for (int i = 0; i < nb_trajets; i++) {
                            if (trajet_get_id(liste_trajets[i]) == id_trajet) {
                                if (voyageur_reserver_trajet(utilisateur_connecte, liste_trajets[i])) {
                                    printf("Réservation réussie.\n");
                                } else {
                                    printf("Réservation échouée.\n");
                                }
                                break;
                            }
                        }
                    } else if (choix_utilisateur == 2) {
                        printf("Fonction de modification à implémenter.\n");
                    } else if (choix_utilisateur == 3) {
                        voyageur_afficher_reservations(utilisateur_connecte);
                    } else if (choix_utilisateur == 4) {
                        for (int i = 0; i < *nb_voyageurs; i++) {
                            if (liste_voyageurs[i] == utilisateur_connecte) {
                                voyageur_effacer(&liste_voyageurs[i]);
                                for (int j = i; j < *nb_voyageurs - 1; j++) {
                                    liste_voyageurs[j] = liste_voyageurs[j + 1];
                                }
                                (*nb_voyageurs)--;
                                utilisateur_connecte = NULL;
                                printf("Compte supprimé.\n");
                                break;
                            }
                        }
                        break;
                    }
                } while (choix_utilisateur != 5 && utilisateur_connecte);
                utilisateur_connecte = NULL;
            } else {
                printf("Identifiant inconnu.\n");
            }
        } else if (choix == 2) {
            if (utilisateur_connecte) {
                printf("Déconnectez-vous avant de créer un autre compte.\n");
            } else {
                if (*nb_voyageurs >= 100) {
                    printf("Nombre maximum de voyageurs atteint.\n");
                    continue;
                }
                char nom[TAILLE_NOM];
                int id;
                printf("Entrez votre nom : ");
                scanf("%s", nom);
                printf("Choisissez un identifiant numérique : ");
                scanf("%d", &id);
                if (voyageur_rechercher_par_id(id, liste_voyageurs, *nb_voyageurs)) {
                    printf("Identifiant déjà utilisé.\n");
                } else {
                    Voyageur nouveau = voyageur_init(nom, id);
                    liste_voyageurs[*nb_voyageurs] = nouveau;
                    (*nb_voyageurs)++;
                    printf("Compte créé. Connectez-vous pour continuer.\n");
                }
            }
        }
    } while (choix != 3);
}

