#ifndef DB_H
#define DB_H

#include <sqlite3.h>

extern sqlite3 *db; // Declare the global database pointer

int db_init(const char *filename); // Function to open the DB
void db_close();                   // Function to close the DB
int create_users_table();

#endif
