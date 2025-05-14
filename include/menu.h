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
void changer_mot_de_passe();
void mettre_a_jour_infos_controleur();

#endif // MENU_H