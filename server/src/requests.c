#include "requests.h"
#include "database.h"
#include "jansson.h"
#include <string.h>

static const char *login_handler(json_t *request) {
  const char *response = NULL;

  // extract informations
  const char *username =
      json_string_value(json_object_get(request, "username"));
  const char *password =
      json_string_value(json_object_get(request, "password"));

  // query for user
  const char *sql = "SELECT password FROM users WHERE username = ?;";
  sqlite3_stmt *stmt;
  int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
  if (rc != SQLITE_OK) {
    fprintf(stderr, "Failed to prepare statement: %s\n", sqlite3_errmsg(db));
    return NULL;
  }

  sqlite3_bind_text(stmt, 1, username, -1, SQLITE_STATIC);

  json_t *ack = json_object();
  json_object_set_new(ack, "ack", json_string("login"));

  rc = sqlite3_step(stmt);
  if (rc == SQLITE_ROW) {

    // User found, get the password
    const unsigned char *db_password = sqlite3_column_text(stmt, 0);

    // if the username and pssword match
    if (strcmp((const char *)db_password, password) == 0) {
      json_object_set_new(ack, "return", json_integer(1));

      // if the password does not match
    } else {
      json_object_set_new(ack, "return", json_integer(0));
    }
    sqlite3_finalize(stmt);

  } else {
    // No such user
    sqlite3_finalize(stmt);
    json_object_set_new(ack, "return", json_integer(0));
  }

  response = json_dumps(ack, 0);
  json_decref(ack);
  return response;
}

static const char *signup_handler(json_t *request) {

  // declare the return string
  const char *response = NULL;

  // extract informations
  const char *username =
      json_string_value(json_object_get(request, "username"));
  const char *nickname =
      json_string_value(json_object_get(request, "nickname"));
  const char *password =
      json_string_value(json_object_get(request, "password"));

  // prepare for insertion
  const char *sql_insert =
      "INSERT INTO users (username, nickname, password) VALUES (?, ?, ?);";
  sqlite3_stmt *stmt;
  int rc = sqlite3_prepare_v2(db, sql_insert, -1, &stmt, 0);
  if (rc != SQLITE_OK) {
    fprintf(stderr, "Failed to prepare statement: %s\n", sqlite3_errmsg(db));
    return NULL;
  }

  // fill the command
  sqlite3_bind_text(stmt, 1, username, -1, SQLITE_STATIC);
  sqlite3_bind_text(stmt, 2, nickname, -1, SQLITE_STATIC);
  sqlite3_bind_text(stmt, 3, password, -1, SQLITE_STATIC);

  rc = sqlite3_step(stmt);

  json_t *ack = json_object();

  json_object_set_new(ack, "ack", json_string("signup"));

  if (rc == SQLITE_CONSTRAINT) {
    json_object_set_new(ack, "return", json_integer(0));
  } else if (rc != SQLITE_DONE) {
    json_object_set_new(ack, "return", json_integer(0));
  } else {
    json_object_set_new(ack, "return", json_integer(1));
  }

  response = json_dumps(ack, 0);

  sqlite3_finalize(stmt);
  json_decref(ack);
  return response;
}

static const char *connect_handler() {
  const char *response = NULL;

  json_t *ack = json_object();
  json_object_set_new(ack, "ack", json_string("connect"));
  response = json_dumps(ack, 0);
  json_decref(ack);

  return response;
}

const char *request_handler(char *buffer) {

  const char *response = NULL;

  json_t *request;
  json_error_t error;

  request = json_loads(buffer, 0, &error);
  if (!request) {
    fprintf(stderr, "error: on line %d: %s\n", error.line, error.text);
    return NULL;
  }

  const char *type = json_string_value(json_object_get(request, "type"));

  if (strcmp(type, "login") == 0) {
    response = login_handler(request);
  } else if (strcmp(type, "signup") == 0) {
    response = signup_handler(request);
  } else if (strcmp(type, "connect") == 0) {
    response = connect_handler();
  } // else if (strcmp(type, "message") == 0) {
  //   response = message_handler(request);
  // } else if (strcmp(type, "add") == 0) {
  //   response = add_handler(request);
  // } else if (strcmp(type, "delete") == 0) {
  //   response = delete_handler(request);
  // } else if (strcmp(type, "block") == 0) {
  //   response = block_handler(request);
  // } else {
  //   // respond to unknown request type
  // }
  //
  json_decref(request);
  return response;
}
