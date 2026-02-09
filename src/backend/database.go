package main

import (
	"database/sql"
	"fmt"

	_ "github.com/mattn/go-sqlite3"
)

func DB_init(dbPath string) error {
	db, err := sql.Open("sqlite3", dbPath)
	if err != nil {
		return err
	}
	defer db.Close()
	db.Exec(`CREATE TABLE IF NOT EXISTS links (longurl TEXT NOT NULL, shorturl TEXT UNIQUE NOT NULL);`)

	return nil
}

func DB_storeLink(dbPath string, longURL string, shortName string) error {
	db, err := sql.Open("sqlite3", dbPath)
	if err != nil {
		return err
	}
	defer db.Close()

	insertSQL := fmt.Sprintf("INSERT INTO links (longurl, shorturl) VALUES ('%s', '%s');", longURL, shortName)

	_, err = db.Exec(insertSQL)
	return err

}

func DB_getMatchingUrl(dbPath string, short string) (string, error) {
	db, err := sql.Open("sqlite3", dbPath)
	if err != nil {
		return "", err
	}
	defer db.Close()

	getSQL := fmt.Sprintf("SELECT * FROM links WHERE shorturl = '%s';", short)
	row := db.QueryRow(getSQL)

	var longUrl string
	var _short string
	err = row.Scan(&longUrl, &_short)
	if err != nil {
		return "", err
	}

	return longUrl, nil
}
