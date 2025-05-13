#include "cJSON.h"
#include "voyageur.h"
#include "trajet.h"

void create_voyageurs_json(cJSON*, VoyageurDB*);
void create_trajets_json(cJSON*, TrajetDB*);
void export_db_to_json(const char* filename);
void export_voyageurs_to_file(const char* filename);
void export_trajets_to_file(const char* filename);