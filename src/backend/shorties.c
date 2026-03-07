#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>

typedef struct Shortie {
    char* short_name;
    char* long_url;
    time_t created_at;
    time_t expires_at;
} Shortie;



// This function creates the shorties file if it does not exist
void shorties_init(char* shorties_path) {
    if (access(shorties_path, F_OK) != 0) {
        FILE* fp = fopen(shorties_path, "a");
        fclose(fp);
    }
}

// Returns the URL associated with a short name (returns NULL if there is none)
char* shorties_expand(char* shorties_path, char* shorty) {
    FILE* shorties = fopen(shorties_path, "r");

    char current_line[512];
    bool found_link = false;

    while (fgets(current_line, sizeof(current_line), shorties)) {
        char* short_name = strtok(current_line, " ");
        if (strcmp(shorty, short_name) == 0) {
            time_t creation_date = atoi(strtok(NULL, " "));
            time_t expiry_date = atoi(strtok(NULL, " "));
            
            // if expiry date has passed, ignore it
            if (difftime(expiry_date, creation_date) <=0 && expiry_date != 0) {
                continue;
            }

            char* long_url = strtok(NULL, " ");
            return long_url;
            
        }
    }

}

void shorties_add(char* shorties_path, Shortie shortie) {
    FILE* shorties = fopen(shorties_path, "a");

    shortie.created_at = time(NULL);

    fprintf(shorties,
        "%s %ld %ld %s\n",
        shortie.short_name,
        shortie.created_at,
        shortie.expires_at,
        shortie.long_url
    );

    fclose(shorties);
}
