#ifndef JSON_EXPORT_H
#define JSON_EXPORT_H

#include "trajet.h"
#include "voyageur.h"

extern TrajetDB trajetDB; // Declare trajetDB as external

void reset_exported_data_json(); // Déclaration de la fonction pour l'utiliser dans main.c ou ailleurs
void export_trajet_to_json(const Trajet *trajet, const char *filename);
void export_voyageurs_to_json(const Voyageur voyageurs[], int count, const char *filename);
void exporter_json(const char *filename);

#endif