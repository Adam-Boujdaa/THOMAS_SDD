#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "voyageur.h"
#include "trajets.h"
#include "json.h"  // Include json-parser library

// Structure to hold our voyageur database
typedef struct {
    Voyageur* voyageurs;
    int count;
} VoyageurDB;

// Function to load voyageurs (mock function since we don't have the actual implementation)
VoyageurDB* load_voyageurs() {
    VoyageurDB* db = (VoyageurDB*)malloc(sizeof(VoyageurDB));
    if (!db) {
        fprintf(stderr, "Failed to allocate memory for voyageur database\n");
        exit(EXIT_FAILURE);
    }
    
    // In a real application, you would load this from a database or file
    // For this example, we'll create some sample data
    db->count = 3;
    db->voyageurs = (Voyageur*)malloc(db->count * sizeof(Voyageur));
    
    if (!db->voyageurs) {
        fprintf(stderr, "Failed to allocate memory for voyageurs\n");
        free(db);
        exit(EXIT_FAILURE);
    }
    
    db->voyageurs[0] = creer_voyageur("Dupont Jean", 1);
    db->voyageurs[1] = creer_voyageur("Martin Sophie", 2);
    db->voyageurs[2] = creer_voyageur("Petit Thomas", 3);
    
    // Assign some trajets (for demonstration)
    reserver_trajet(db->voyageurs[0], 101);
    reserver_trajet(db->voyageurs[0], 102);
    reserver_trajet(db->voyageurs[1], 101);
    reserver_trajet(db->voyageurs[2], 103);
    
    return db;
}

// Function to free voyageur database
void free_voyageur_db(VoyageurDB* db) {
    if (!db) return;
    
    for (int i = 0; i < db->count; i++) {
        liberer_voyageur(db->voyageurs[i]);
    }
    
    free(db->voyageurs);
    free(db);
}

// Function to create a JSON representation of voyageurs for a specific trajet
json_value* create_voyageurs_json(VoyageurDB* db, int trajet_id) {
    if (!db) return NULL;
    
    // Create the root object
    json_value* root = json_object_new(0);
    if (!root) return NULL;
    
    // Add trajet information
    json_object_push(root, "trajet_id", json_integer_new(trajet_id));
    
    // Create voyageurs array
    json_value* voyageurs_array = json_array_new(0);
    if (!voyageurs_array) {
        json_value_free(root);
        return NULL;
    }
    
    // Count how many voyageurs are on this trajet
    int count = 0;
    for (int i = 0; i < db->count; i++) {
        if (trajet_existe(db->voyageurs[i], trajet_id)) {
            count++;
        }
    }
    
    json_object_push(root, "total_voyageurs", json_integer_new(count));
    
    // Add each voyageur who has reserved this trajet
    for (int i = 0; i < db->count; i++) {
        if (trajet_existe(db->voyageurs[i], trajet_id)) {
            // Get a reference to the voyageur for easier access
            Voyageur v = db->voyageurs[i];
            
            // Create voyageur object
            json_value* voyageur_obj = json_object_new(0);
            
            // Since we can't directly access voyageur fields due to opaque pointer,
            // we'll create a function to get the necessary information
            char nom[MAX_NOM];
            int id;
            
            // For this example, we'll hardcode access to the structure
            // In a real application, you would add getter functions to the voyageur API
            struct s_voyageur* v_struct = (struct s_voyageur*)v;
            strcpy(nom, v_struct->nom);
            id = v_struct->id;
            
            // Add voyageur properties
            json_object_push(voyageur_obj, "id", json_integer_new(id));
            json_object_push(voyageur_obj, "nom", json_string_new(nom));
            
            // Add the voyageur object to the array
            json_array_push(voyageurs_array, voyageur_obj);
        }
    }
    
    // Add the voyageurs array to the root object
    json_object_push(root, "voyageurs", voyageurs_array);
    
    return root;
}

// Function to export voyageurs on a trajet to a JSON file
void export_voyageurs_to_json(VoyageurDB* db, int trajet_id, const char* filename) {
    if (!db || !filename) return;
    
    // Create JSON representation
    json_value* json = create_voyageurs_json(db, trajet_id);
    if (!json) {
        fprintf(stderr, "Failed to create JSON\n");
        return;
    }
    
    // Open file for writing
    FILE* file = fopen(filename, "w");
    if (!file) {
        fprintf(stderr, "Failed to open file for writing: %s\n", filename);
        json_value_free(json);
        return;
    }
    
    // Serialize JSON to string
    // This is a simplified approach - in a real application you would use
    // a proper JSON serialization function from the json-parser library
    fprintf(file, "{\n");
    fprintf(file, "  \"trajet_id\": %d,\n", trajet_id);
    fprintf(file, "  \"total_voyageurs\": %d,\n", count);
    fprintf(file, "  \"voyageurs\": [\n");
    
    // Write each voyageur
    int count = 0;
    for (int i = 0; i < db->count; i++) {
        if (trajet_existe(db->voyageurs[i], trajet_id)) {
            struct s_voyageur* v = (struct s_voyageur*)db->voyageurs[i];
            
            if (count > 0) {
                fprintf(file, ",\n");
            }
            
            fprintf(file, "    {\n");
            fprintf(file, "      \"id\": %d,\n", v->id);
            fprintf(file, "      \"nom\": \"%s\"\n", v->nom);
            fprintf(file, "    }");
            
            count++;
        }
    }
    
    fprintf(file, "\n  ]\n");
    fprintf(file, "}\n");
    
    // Clean up
    fclose(file);
    json_value_free(json);
    
    printf("Successfully exported %d voyageurs from trajet %d to %s\n", 
           count, trajet_id, filename);
}

int main(int argc, char* argv[]) {
    // Check command line arguments
    if (argc != 3) {
        printf("Usage: %s <trajet_id> <output_file.json>\n", argv[0]);
        return 1;
    }
    
    // Parse trajet_id
    int trajet_id = atoi(argv[1]);
    if (trajet_id <= 0) {
        fprintf(stderr, "Invalid trajet ID: %s\n", argv[1]);
        return 1;
    }
    
    const char* output_file = argv[2];
    
    // Load voyageur database
    VoyageurDB* db = load_voyageurs();
    
    // Export voyageurs to JSON
    export_voyageurs_to_json(db, trajet_id, output_file);
    
    // Clean up
    free_voyageur_db(db);
    
    return 0;
} 