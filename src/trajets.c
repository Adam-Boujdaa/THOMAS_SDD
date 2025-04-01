#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/trajets.h"

#define MAX_NOM_VILLE 85 // Longueur maximale du nom d'une ville en France


// Def Structure 

typedef struct s_trajet {
    char gare_depart[MAX_NOM_VILLE]; // Nom de la gare de départ
    char gare_arrivee[MAX_NOM_VILLE]; // Nom de la gare d'arrivée
    float duree; // Durée totale du trajet en heures
    char** liste_gares; // Liste des gares intermédiaires (tableau dynamique de chaînes)
    int nombre_gares; // Nombre de gares intermédiaires
} Trajet;
*


// Constructeur 

Trajet* trajet_init(); // Initialise un trajet vide avec des valeurs par défaut.


// Gestion des gares dans un trajet

void trajet_ajouter_gare(Trajet* trajet, const char* nom_gare, float temps_depuis_depart); // Ajoute une gare intermédiaire.
void trajet_supprimer_gare(Trajet* trajet, const char* nom_gare); // Supprime une gare intermédiaire.
void trajet_modifier_temps(Trajet* trajet, const char* nom_gare, float nouveau_temps); // Modifie le temps d'arrivée à une gare.



//Manipulation des trajets

Trajet* trajet_fusionner(const Trajet* trajet1, const Trajet* trajet2); // Fusionne deux trajets en un seul.
Trajet* trajet_prolonger(Trajet* trajet, const char* nouvelle_gare, float temps_supplementaire); // Ajoute une nouvelle gare à la fin du trajet.
Trajet* trajet_raccourcir(Trajet* trajet, const char* gare_finale); // Raccourcit le trajet jusqu'à une gare spécifique.


//Gestion des fichiers
void trajet_sauvegarder(const Trajet* trajet, const char* nom_fichier); // Sauvegarde les informations du trajet dans un fichier.
Trajet* trajet_charger(const char* nom_fichier); // Charge les informations d'un trajet depuis un fichier.


// Affichage et recherche

void trajet_afficher(const Trajet* trajet); // Affiche les informations du trajet (gare de départ, arrivée, durée, gares intermédiaires).
Trajet* trajet_rechercher(const char* gare_depart, const char* gare_arrivee); // Recherche un trajet entre deux gares spécifiques (implémentation dépendante).


//Fonctions pratiques

int trajet_nombre_gares(const Trajet* trajet); // Retourne le nombre total de gares (intermédiaires incluses).
float trajet_duree_totale(const Trajet* trajet); // Calcule la durée totale du trajet en heures.
int trajet_est_vide(const Trajet* trajet); // Vérifie si le trajet est vide (aucune gare définie).
void trajet_effacer(Trajet** trajet); // Libère la mémoire allouée pour un objet `Trajet`.

//juste au cas où 

Trajet just_in_case1(Trajet t1, Trajet t2, int x, int y, float a, float b, char * idk, char * jsp);
Trajet just_in_case2(Trajet t1, Trajet t2, int x, int y, float a, float b, char * idk, char * jsp);
void fct_de_schrodinger1(void* arg1, void* arg2);
void fct_de_schrodinger2(void* arg1, void* arg2);
int just_in_case3(int a, int b);
float just_in_case4(float a, float b);







