#include "libclient.h"
#include "communicate.h"
#include "jansson.h"
#include <arpa/inet.h>
#include <errno.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>

int sock_fd = -1;

extern int cus_write(int fd, const char *message) {
  // prepare the length header
  size_t msg_len = strlen(message);
  uint32_t netlen = htonl(msg_len);

  // write out the 4-byte length prefix
  const char *p = (const char *)&netlen;
  size_t left = sizeof(netlen);
  while (left > 0) {
    ssize_t n = write(fd, p, left);
    if (n < 0) {
      if (errno == EINTR)
        continue; // retry on interrupt
      return 0;   // real error
    }
    if (n == 0)
      return 0; // peer closed
    p += n;
    left -= n;
  }

  // write out the message payload
  p = message;
  left = msg_len;
  while (left > 0) {
    ssize_t n = write(fd, p, left);
    if (n < 0) {
      if (errno == EINTR)
        continue;
      return 0;
    }
    if (n == 0)
      return 0;
    p += n;
    left -= n;
  }

  return 1;
}

extern const char *cus_read(int fd) {
  uint32_t netlen;
  char *ptr = (char *)&netlen;
  size_t left = sizeof(netlen);

  // read the 4-byte length header
  while (left > 0) {
    ssize_t n = read(fd, ptr, left);
    if (n < 0) {
      if (errno == EINTR)
        continue;
      return NULL;
    }
    if (n == 0) // peer closed
      return NULL;
    ptr += n;
    left -= n;
  }

  // convert length to host order
  uint32_t len = ntohl(netlen);

  // reject crazy lengths (optional safeguard)
  if (len > 10 * 1024 * 1024) // e.g. 10 MB max
    return NULL;

  // allocate buffer for payload + NUL
  char *buf = malloc(len + 1);
  if (!buf)
    return NULL;

  // read exactly 'len' bytes of payload
  ptr = buf;
  left = len;
  while (left > 0) {
    ssize_t n = read(fd, ptr, left);
    if (n < 0) {
      if (errno == EINTR)
        continue;
      free(buf);
      return NULL;
    }
    if (n == 0) { // unexpected EOF
      free(buf);
      return NULL;
    }
    ptr += n;
    left -= n;
  }

  // null-terminate and return
  buf[len] = '\0';
  const char *out = buf;
  return out;
}

int client_connect(const char *address, int port) {
  struct sockaddr_in serv_addr;

  // create socket
  if ((sock_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    return -1;
  }

  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons(port);

  // convert IPv4 address from text to binary form
  if (inet_pton(AF_INET, address, &serv_addr.sin_addr) <= 0) {
    return -1;
  }

  // connect to server
  if (connect(sock_fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
    return -1;
  }

  json_t *connect = json_object();

  json_object_set_new(connect, "type", json_string("connect"));

  const char *test = json_dumps(connect, 0);

  cus_write(sock_fd, test);

  json_decref(connect);

  char *buffer = (char *)cus_read(sock_fd);

  json_t *ack;
  json_error_t error;

  ack = json_loads(buffer, 0, &error);
  if (!ack) {
    free(buffer);
    return -3;
  }

  const char *acknowledgement = json_string_value(json_object_get(ack, "ack"));

  if (strcmp(acknowledgement, "connect") == 0) {
    json_decref(ack);
    return 0;
  } else {
    json_decref(ack);
    return 1;
  }
}

void client_close() {
  close(sock_fd);
  sock_fd = -1;
}
