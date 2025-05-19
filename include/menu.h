#ifndef MENU_H
#define MENU_H

void display_menu();
void handle_choice(int choice);

// Add function prototypes for undefined functions
int verifier_mot_de_passe_admin(const char* mot_de_passe);
int verifier_mot_de_passe_controleur(const char* mot_de_passe);
int verifier_identifiants_utilisateur(const char* utilisateur, const char* mot_de_passe);
void rechercher_trajet();
void rechercher_et_choisir_trajet();
void reserver_ou_modifier_trajet();
void creer_compte();
void exporter_donnees();
void mettre_a_jour_infos_controleur();
void reserver_place_voyageur();
void rechercher_et_reserver_trajet();
void rechercher_trajet_txt();
void rechercher_trajet_txt_controleur();
void annuler_reservation();
void reserver_trajet_par_numero();

#endif // MENU_H