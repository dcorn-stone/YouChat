#include "database.h"
#include "requests.h"
#include <arpa/inet.h>
#include <errno.h>
#include <netinet/in.h>
#include <sqlite3.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define PORT 8080

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

  // main loop
  while (1) {

    // connect to client
    client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &addrlen);
    if (client_fd < 0) {
      perror("Failed to accept connection");
      continue;
    }

    u_int32_t size;
    if (read(client_fd, &size, sizeof(size)) != sizeof(size)) {
      perror("Error when recieving message");
      return -6;
    }
    size = ntohl(size); // convert from network byte order

    char *buffer = malloc(size + 1);
    if (!buffer)
      return -7;

    // recieve requests and respond
    memset(buffer, 0, size + 1);
    size_t bytes_read = read(client_fd, buffer, size + 1);
    if (bytes_read > 0) {
      printf("Request recieved: \n%s\n", buffer);
      buffer[bytes_read] = '\0';
    }
    // prepare response and send
    const char *response = request_handler(buffer);
    uint32_t len = htonl(strlen(response));
    write(client_fd, &len, sizeof(len));
    write(client_fd, response, strlen(response));

    close(client_fd);
    free(buffer);
  }

  close(server_fd);
  db_close();
  return 0;
}
