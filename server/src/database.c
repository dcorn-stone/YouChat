#include "database.h"
#include <stdio.h>

sqlite3 *db = NULL; // Define the global pointer

int db_init(const char *filename) { return sqlite3_open(filename, &db); }

void db_close() {
  if (db) {
    sqlite3_close(db);
    db = NULL;
  }
}

int create_users_table() {
  const char *sql = "CREATE TABLE IF NOT EXISTS users ("
                    "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                    "username TEXT UNIQUE NOT NULL, "
                    "nickname TEXT NOT NULL, "
                    "password TEXT NOT NULL);";
  char *err_msg = NULL;
  int rc = sqlite3_exec(db, sql, 0, 0, &err_msg);
  if (rc != SQLITE_OK) {
    fprintf(stderr, "SQL error while creating users table: %s\n", err_msg);
    sqlite3_free(err_msg);
    return rc;
  }
  return SQLITE_OK;
}
