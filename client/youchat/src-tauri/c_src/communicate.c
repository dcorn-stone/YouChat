#include "communicate.h"
#include <arpa/inet.h>
#include <jansson.h>
#include <string.h>
#include <unistd.h>

int signup(const char *username, const char *nickname, const char *password) {

  // create json object for request
  json_t *req = json_object();

  json_object_set_new(req, "type", json_string("signup"));
  json_object_set_new(req, "username", json_string(username));
  json_object_set_new(req, "nickname", json_string(nickname));
  json_object_set_new(req, "password", json_string(password));

  const char *response = json_dumps(req, 0);

  uint32_t len = htonl(strlen(response));
  write(sock_fd, &len, sizeof(len));
  write(sock_fd, response, strlen(response));

  // clean up
  json_decref(req);

  u_int32_t size;

  if (read(sock_fd, &size, sizeof(size)) != sizeof(size)) {
    return -1;
  }

  size = ntohl(size);

  char *buffer = malloc(size + 1);
  if (!buffer)
    return -2;

  // recieve acknowledgement
  memset(buffer, 0, size + 1);
  size_t bytes_read = read(sock_fd, buffer, size);
  if (bytes_read > 0) {
    buffer[bytes_read] = '\0';
  }

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
