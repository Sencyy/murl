package main

import (
	"database/sql"
	"fmt"

	_ "github.com/mattn/go-sqlite3"
)

func DB_init(dbPath string) error {
	db, err := sql.Open("sqlite3", "./database.db")
	if err != nil {
		return err
	}
	defer db.Close()
	db.Exec(`CREATE TABLE IF NOT EXISTS links (longurl TEXT NOT NULL, shorturl TEXT UNIQUE NOT NULL);`)

	return nil
}

func DB_storeLink(longURL string, shortName string) error {
	db, err := sql.Open("sqlite3", "./database.db")
	if err != nil {
		return err
	}
	defer db.Close()

	insertSQL := fmt.Sprintf("INSERT INTO links (longurl, shorturl) VALUES ('%s', '%s');", longURL, shortName)

	_, err = db.Exec(insertSQL)
	return err

}
