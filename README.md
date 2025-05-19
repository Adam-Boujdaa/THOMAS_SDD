# Gestion de trajets et voyageurs en C

Ce projet est une application console en C permettant de gérer une compagnie de chemin de fer : création de trajets, gestion des voyageurs, réservations, et administration sécurisée.

## Fonctionnalités principales
- **Gestion des utilisateurs** :
  - Création de comptes voyageurs (mots de passe hashés SHA256 dans `users.txt`)
  - Authentification voyageurs, contrôleur et administrateur
  - Changement de mot de passe (voyageur, admin, contrôleur)
- **Gestion des trajets** :
  - Ajout, modification, suppression de trajets (par l’admin)
  - Stockage des trajets dans `trajet.txt`
- **Réservation** :
  - Recherche de trajets par gares de départ/arrivée
  - Réservation de places par les voyageurs
- **Exportation** :
  - Export des données au format JSON

## Structure des fichiers
- `users.txt` : utilisateurs voyageurs (login + hash du mot de passe)
- `admin.txt` : identifiant et mot de passe admin (en clair)
- `controleur.txt` : identifiant et mot de passe contrôleur (en clair)
- `trajet.txt` : liste des trajets (voir exemple ci-dessous)
- `src/` et `include/` : code source et headers

## Exemple de format `trajet.txt`
```
Fusion
3
Paris 0.0
Lyon 2.0
Marseille 3.0
SudExpress
4
Toulouse 0.0
Carcassonne 1.0
Montpellier 2.5
Nice 5.0
```

## Installation des bibliothèques
Debian/Ubuntu
```sh
sudo apt-get install libcjson-dev
```
Arch
```sh
sudo pacman -S cjson
```


## Compilation et exécution

```sh
make
./bin/programme
```

## Utilisation
- Lancer le programme puis choisir le rôle (admin, contrôleur, voyageur)
- Suivre les menus pour gérer trajets, comptes, réservations, etc.

## Sécurité
- Les mots de passe voyageurs sont hashés (SHA256)
- Les mots de passe admin/contrôleur sont stockés en clair (pour compatibilité avec l’énoncé)

## Auteurs
- BOUDOUHI-MEZROUI Ayoub
- BOUJDAA Adam
- SAHLI Aziz

## Licence
MIT
