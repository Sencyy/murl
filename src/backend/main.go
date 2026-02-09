package main

import (
	"fmt"
	"net/http"
	"os"

	"github.com/kpechenenko/rword"
)

type Response struct {
	success  bool
	shortUrl string
}

var SERVER_PORT string
var DB_PATH string

func shortenLink(w http.ResponseWriter, req *http.Request) {
	longURL := req.FormValue("url")
	host := req.Header.Get("Host")

	g, err := rword.NewWithDict("./dict")
	if err != nil {
		w.WriteHeader(http.StatusInternalServerError)
		fmt.Printf("ERROR: %s", err)
		return
	}
	shortLink := g.Word()

	err = DB_storeLink(longURL, shortLink)
	if err != nil {
		fmt.Printf("ERROR: %s", err)

		for err.Error() == "UNIQUE constraint failed: links.shorturl" { // Keep trying a new word until it finds an unused one
			shortLink = g.Word()
			err = DB_storeLink(longURL, shortLink)
		}
	}

	if err != nil {
		fmt.Printf("ERROR: %s", err)
	}

	w.WriteHeader(http.StatusOK)
	w.Header().Set("Content-Type", "application/json")
	fmt.Fprintf(w, `{ "success": true, "shortUrl": "%s" }`, fmt.Sprintf("https://%s/%s", host, shortLink))
}

func main() {
	SERVER_PORT = os.Getenv("MURL_SERVER_PORT")
	if path := os.Getenv("MURL_DB_PATH"); path == "" {
		DB_PATH = "./database.db"
	} else {
		DB_PATH = path
	}

	DB_init(DB_PATH)

	fmt.Println("Listening on port 8080")
	http.HandleFunc("/shrink", shortenLink)
	http.ListenAndServe(":8080", nil)
}
