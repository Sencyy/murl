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

void shorties_init(char* shorties_path);

char* shorties_expand(char* shorties_path, char* shorty);

void shorties_add(char* shorties_path, Shortie shortie);

