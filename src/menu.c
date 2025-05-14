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

void handle_choice(int choice) {
    char username[50], password[50];
    switch (choice) {
        case 1:
            printf("Entrez le mot de passe Administrateur: ");
            scanf("%s", password);
            if (check_admin_password(password)) {
                printf("1) Gestion des trajets\n");
                printf("2) Exportation des trajets et des voyageurs\n");
                printf("3) Administration\n");
                int admin_choice;
                scanf("%d", &admin_choice);
                switch (admin_choice) {
                    case 1:
                        manage_trips();
                        break;
                    case 2:
                        export_data();
                        break;
                    case 3:
                        change_password();
                        break;
                    default:
                        printf("Choix invalide.\n");
                }
            } else {
                printf("Mot de passe incorrect.\n");
            }
            break;
        case 2:
            printf("Entrez le mot de passe Controleur: ");
            scanf("%s", password);
            if (check_controller_password(password)) {
                printf("4) Recherche\n");
                printf("5) Administration\n");
                int controller_choice;
                scanf("%d", &controller_choice);
                switch (controller_choice) {
                    case 4:
                        search_trip();
                        break;
                    case 5:
                        update_controller_info();
                        break;
                    default:
                        printf("Choix invalide.\n");
                }
            } else {
                printf("Mot de passe incorrect.\n");
            }
            break;
        case 3:
            printf("Entrez votre nom d'utilisateur: ");
            scanf("%s", username);
            printf("Entrez votre mot de passe: ");
            scanf("%s", password);
            if (check_user_credentials(username, password)) {
                printf("6) Recherche et Choix d’un trajet\n");
                printf("7) Réservation/Modification d’un trajet\n");
                int user_choice;
                scanf("%d", &user_choice);
                switch (user_choice) {
                    case 6:
                        search_and_choose_trip();
                        break;
                    case 7:
                        book_or_modify_trip();
                        break;
                    default:
                        printf("Choix invalide.\n");
                }
            } else {
                printf("Nom d'utilisateur ou mot de passe incorrect.\n");
            }
            break;
        case 4:
            create_account();
            printf("6) Recherche et Choix d’un trajet\n");
            printf("7) Réservation/Modification d’un trajet\n");
            int new_user_choice;
            scanf("%d", &new_user_choice);
            switch (new_user_choice) {
                case 6:
                    search_and_choose_trip();
                    break;
                case 7:
                    book_or_modify_trip();
                    break;
                default:
                    printf("Choix invalide.\n");
            }
            break;
        default:
            printf("Choix invalide.\n");
            break;
    }
}