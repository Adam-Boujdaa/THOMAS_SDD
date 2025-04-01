#ifndef __TRAJET_H__
#define __TRAJET_H__

// Définir la structure
typedef struct s_trajet * Trajet;

// Constructeur
Trajet trajet_init(); // Crée et initialise un trajet vide

// Opérateurs : Gestion des gares dans un trajet
void trajet_ajouter_gare(Trajet trajet, char* nom_gare, float temps_depuis_depart); // Ajoute une gare au trajet
void trajet_supprimer_gare(Trajet trajet, char* nom_gare); // Supprime une gare du trajet
void trajet_modifier_temps(Trajet trajet, char* nom_gare, float nouveau_temps); // Modifie le temps d'arrivée à une gare

// Opérateurs : Manipulation des trajets
Trajet trajet_fusionner(Trajet trajet1, Trajet trajet2); // Fusionne deux trajets
Trajet trajet_prolonger(Trajet trajet, char* nouvelle_gare, float temps_supplementaire); // Prolonge un trajet avec une nouvelle gare
Trajet trajet_raccourcir(Trajet trajet, char* gare_finale); // Raccourcit un trajet jusqu'à une gare spécifique

// Gestion des fichiers
void trajet_sauvegarder(Trajet trajet, char* nom_fichier); // Sauvegarde un trajet dans un fichier
Trajet trajet_charger(char* nom_fichier); // Charge un trajet depuis un fichier

// Affichage et recherche
void trajet_afficher(Trajet trajet); // Affiche les détails d'un trajet
Trajet trajet_rechercher(char* gare_depart, char* gare_arrivee); // Recherche un trajet entre deux gares

#endif // __TRAJET_H__
