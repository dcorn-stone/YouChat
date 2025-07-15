#include "communicate.h"
#include "libclient.h"
#include <arpa/inet.h>
#include <errno.h>
#include <jansson.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int signup(const char *username, const char *nickname, const char *password) {

  // create json object for request
  json_t *req = json_object();

  json_object_set_new(req, "type", json_string("signup"));
  json_object_set_new(req, "username", json_string(username));
  json_object_set_new(req, "nickname", json_string(nickname));
  json_object_set_new(req, "password", json_string(password));

  char *response = json_dumps(req, 0);

  cus_write(sock_fd, response);

  free(response);
  // clean up
  json_decref(req);

  char *buffer = (char *)cus_read(sock_fd);

  json_t *ack;
  json_error_t error;

  ack = json_loads(buffer, 0, &error);
  if (!ack) {
    free(buffer);
    return -3;
  }

  const char *acknowledgement = json_string_value(json_object_get(ack, "ack"));

  if (strcmp(acknowledgement, "signup") == 0) {
    const json_int_t status =
        json_integer_value(json_object_get(ack, "return"));
    // status will be 1 if sign up is successful
    if (status) {
      json_decref(ack);
      free(buffer);

      return 1;
    } else {
      json_decref(ack);
      free(buffer);

      return 0;
    }
  }

  json_decref(ack);
  free(buffer);
  return 0;
}

int login(const char *username, const char *password) {

  // create json object for request
  json_t *req = json_object();

  json_object_set_new(req, "type", json_string("login"));
  json_object_set_new(req, "username", json_string(username));
  json_object_set_new(req, "password", json_string(password));

  const char *response = json_dumps(req, 0);

  cus_write(sock_fd, response);

  // clean up
  json_decref(req);

  char *buffer = (char *)cus_read(sock_fd);

  json_t *ack;
  json_error_t error;

  ack = json_loads(buffer, 0, &error);
  if (!ack) {
    free(buffer);
    return -3;
  }

  const char *acknowledgement = json_string_value(json_object_get(ack, "ack"));

  if (strcmp(acknowledgement, "login") == 0) {

    const json_int_t status =
        json_integer_value(json_object_get(ack, "return"));

    // status will be 1 if login is successful
    if (status) {
      json_decref(ack);
      free(buffer);
      return 1;

    } else {
      json_decref(ack);
      free(buffer);
      return 0;
    }
  }

  json_decref(ack);
  free(buffer);
  return 0;
}
