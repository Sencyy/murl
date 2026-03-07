#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
// #define _POSIX_C_SOURCE 199309L

#define MAX_WORD_LENGTH 128 

typedef struct Dictionary {
	char** word_list;
	size_t length;
} Dictionary;

Dictionary dict_open(char* filepath) {
	FILE* dict_file = fopen(filepath, "r");

	char** word_list = NULL;
	char buffer[MAX_WORD_LENGTH];
	size_t dict_length = 0;
	
	while (fgets(buffer, sizeof(buffer), dict_file)) {
		word_list = realloc(word_list, sizeof(char*) * dict_length + 1);
		buffer[strcspn(buffer, "\n")] = 0;
		word_list[dict_length] = strdup(buffer);
		dict_length++;
	}
	// As far as my experimentations went, it always ended up counting one line more than the actual file has
	dict_length--;

	printf("%ld words found.\n", dict_length);

	Dictionary dict;
	dict.word_list = word_list;
	dict.length = dict_length;

	return dict;
}

char* dict_random_word(Dictionary dict) {
	struct timespec current_time;
	clock_gettime(CLOCK_TAI, &current_time);
	srand(current_time.tv_nsec);
	size_t word_number = rand() % (dict.length + 1);
	printf("word_number => %ld\n", word_number); // debug
	return dict.word_list[word_number];
}
