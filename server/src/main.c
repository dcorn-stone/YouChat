#include "database.h"
#include "requests.h"
#include <arpa/inet.h>
#include <errno.h>
#include <netinet/in.h>
#include <sqlite3.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define PORT 8080

extern int cus_write(int fd, const char *message) {
  // prepare the length header
  size_t msg_len = strlen(message);
  uint32_t netlen = htonl((uint32_t)msg_len);

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

int main() {

  if (db_init("chat.db") != SQLITE_OK) {
    fprintf(stderr, "Can't open database\n");
    return -1;
  }

  if (create_users_table() != SQLITE_OK) {
    fprintf(stderr, "Failed to create users table\n");
    db_close();
    return -1;
  }

  int server_fd = socket(AF_INET, SOCK_STREAM, 0);

  if (server_fd < 0) {
    perror("Failed to create socket");
    return -2;
  }

  struct sockaddr_in server_addr;

  // using ipv4
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(PORT);

  if (inet_pton(AF_INET, "0.0.0.0", &server_addr.sin_addr) <= 0) {
    perror("inet_pton");
    return -3;
  }

  // bing to a port and an address
  if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) <
      0) {
    perror("Bind failed");
    close(server_fd);
    return -4;
  }

  // listen
  if (listen(server_fd, 3) < 0) {
    perror("Failed to listen");
    close(server_fd);
    return -5;
  }

  // console
  printf("Server started on port %d\n", PORT);
  printf("Listening from %s\n\n", "0.0.0.0");

  // preparing for client connections
  int client_fd;
  struct sockaddr_in client_addr;
  socklen_t addrlen = sizeof(client_addr);

  // connect to client
  client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &addrlen);
  if (client_fd < 0) {
    perror("Failed to accept connection");
  }

  // main loop
  while (1) {

    char *buffer = (char *)cus_read(client_fd);

    printf("Request recieved: %s\n", buffer);

    // prepare response and send
    const char *response = request_handler(buffer);
    if (!response) {
      printf("Response is a NULL porinter");
    } else {

      printf("Response: %s\n", response);
      cus_write(client_fd, response);
    }

    free(buffer);
  }

  close(client_fd);
  close(server_fd);
  db_close();
  return 0;
}
