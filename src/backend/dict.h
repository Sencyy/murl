#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_WORD_LENGTH 128 

typedef struct Dictionary {
	char** word_list;
	size_t length;
} Dictionary;

Dictionary dict_open(char* filepath);

char* dict_random_word(Dictionary dict);
