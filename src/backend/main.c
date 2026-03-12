#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdbool.h>
#include "dict.c"
#include "shorties.c"

#define SERVER_PORT 6946
// The number maximum of connections to be queued
#define LISTEN_BACKLOG 10
#define DEFAULT_MESSAGE "Hello, World!"

#define DEFAULT_DICT_PATH "$out/contrib/dict"
#define DEFAULT_FRONTEND_PATH "$out/frontend/index.html"
#define DEFAULT_SHORTIES_PATH "$out/data/shorties"


// Here we are defining messages for possible HTTP responses
#define HTTP_400 "HTTP/1.1 400 Bad Request\n"
#define HTTP_200 "HTTP/1.1 200 OK\n"
#define HTTP_404 "HTTP/1.1 404 Not Found\n"
#define HTTP_301 "HTTP/1.1 301 Permanent Redirect\n"

int main() {
	// Getting environment variable
	const char* env_dict_path 	  =		getenv("MURL_DICT_PATH");
	const char* env_frontend_path = 	getenv("MURL_FRONTEND_PATH");
	const char* env_shorties_path = 	getenv("MURL_SHORTIES_PATH");

	char* DICT_PATH;
	char* FRONTEND_PATH;
	char* SHORTIES_PATH;

	if (env_dict_path) {
		DICT_PATH = env_dict_path;
	} else {
		DICT_PATH = DEFAULT_DICT_PATH;
	}

	if (env_frontend_path) {
		FRONTEND_PATH = env_frontend_path;
	} else {
		FRONTEND_PATH = DEFAULT_FRONTEND_PATH;
	}

	if (env_shorties_path) {
		SHORTIES_PATH = env_shorties_path;
	} else {
		SHORTIES_PATH = DEFAULT_SHORTIES_PATH;
	}

	// Reading dictionary of words (see dict.c)
	printf("Loading dictionary... ");
	Dictionary dict = dict_open(DICT_PATH);
	// ------------------------------------------------------------

	// Loading index.html into memory
	FILE* index_file = fopen(FRONTEND_PATH, "r");
	struct stat index_stat;
			
	// determining index size
	stat(FRONTEND_PATH, &index_stat);
	long index_size = index_stat.st_size;

	// allocating memory and saving the contents
	char* index = malloc(index_size);
	fread(index, 1, index_size, index_file);

	printf("Loaded: index.html (%ldKB)\n", index_size / 1024);
//  ---------------------------------------------------------------

	// Initializing short link database (see shorties.c)
	shorties_init(SHORTIES_PATH);
//  ---------------------------------------------------------------

	// Creating socket and binding it to SERVER_PORT
	struct sockaddr_in sock_addr;
	sock_addr.sin_family = AF_INET;
	sock_addr.sin_port = htons(SERVER_PORT); // The port to listen
	sock_addr.sin_addr.s_addr = INADDR_ANY;

	int s = socket(AF_INET, SOCK_STREAM, 0);
	bind(s, (struct sockaddr *)&sock_addr, sizeof(sock_addr));
	int shit = listen(s, LISTEN_BACKLOG);
//  ---------------------------------------------------------------


	// Registering all the valid paths
	char valid_paths[5][64] = {
		"/",
		"/index.html",
		"/shrink"
	};
//  --------------------------------------------------------------


	// The main application loop (that will handle every request)
	while (true) {
		int client_sock = accept(s, NULL, NULL);
		
	// Everything from here will run at every request
		printf("Network request received\n");

	// storing the request in a variable
		char http_request[256];
		memset(http_request, 0, 256); // Fill the array with zeroes (null chars) to avoid issues

		recv(client_sock, http_request, 255, 0);
	//  ---------------------------------------------------------

		printf("%s\n", http_request);
		
	// Checking if request is GET, if not we have no business with it
		char *http_method = strtok(http_request, " ");
		printf("METHOD = %s\n", http_method);
		
		if (strcmp(http_method, "GET") != 0) { // if method is not get
			send(client_sock, HTTP_400, strlen(HTTP_400), 0);
			close(client_sock);
			continue; // skips the loop and waits for a new connection				
		}
	//  ---------------------------------------------------------
	
		char *http_path = strtok(NULL, " ");
		printf("PATH = %s\n", http_path);	
		
		// Identifying shrink function
		if (strncmp(http_path, "/shrink", 7) == 0) { // if path starts with "/shrink"...
			strtok(http_path, "?");
			char *long_url = strtok(NULL, "?");
			printf("Long URL => %s\n", long_url);

			char* short_url = dict_random_word(dict);
			printf("Short URL => %s\n", short_url);

			// adding link to the database
			Shortie shortie;
			shortie.long_url = long_url;
			shortie.short_name = short_url;

			// For now we are not implementing the ability to make links expire
			shortie.expires_at = 0;

			shorties_add(SHORTIES_PATH, shortie);
		
			char json_content[strlen("{\"success\": true,\"shortUrl\": \"%s\"}") + strlen(short_url)];
			sprintf(json_content, "{\"success\": true,\"shortUrl\": \"%s\"}", short_url);
			printf("JSON => %s\n", json_content);

			char* headers = "Content-type: application/json; charset:utf-8\nContent-length: ";
			send(client_sock, HTTP_200, sizeof(HTTP_200), 0);
			
			// sending the headers
			char length[20];
			sprintf(length, "%ld", strlen(json_content));
			write(client_sock, headers, strlen(headers));
			write(client_sock, length, strlen(length));
			write(client_sock, "\n\n", strlen("\n\n"));
			write(client_sock, json_content, strlen(json_content));
		} else if (strcmp(http_path, "/") == 0 || strcmp(http_path, "/index.html") == 0) {
			
			char* headers = "Content-type: text/html; charset:utf-8\nContent-length: ";
			char length[20];
			sprintf(length, "%ld", index_size);

			write(client_sock, HTTP_200, strlen(HTTP_200));

			write(client_sock, headers, strlen(headers));
			write(client_sock, length, strlen(length));

			write(client_sock, "\n\n", strlen("\n\n"));

			write(client_sock, index, strlen(index));
		} else { // if the client didn't ask for shrink nor index...
			char* full_url = shorties_expand(SHORTIES_PATH, http_path + 1);
			if (full_url) {
				write(client_sock, HTTP_301, strlen(HTTP_301));

				char headers[256];
				sprintf(headers, "Location: %s\nContent-length: 0", full_url);

				write(client_sock, headers, strlen(headers));
				write(client_sock, "\n\n", strlen("\n\n"));

			}
		}
	
		
		// send(client_sock, DEFAULT_MESSAGE, strlen(DEFAULT_MESSAGE), 0);

		close(client_sock);
	}
	


	return 0;
}
