#include "libclient.h"
#include "communicate.h"
#include <arpa/inet.h>
#include <unistd.h>

int sock_fd = -1;

int client_connect() {
  struct sockaddr_in serv_addr;

  // Create socket
  if ((sock_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    return -1;
  }

  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons(PORT);

  // Convert IPv4 address from text to binary form
  if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
    return -1;
  }

  // Connect to server
  if (connect(sock_fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
    return -1;
  }

  return 0;
}

void client_close() { close(sock_fd); }
