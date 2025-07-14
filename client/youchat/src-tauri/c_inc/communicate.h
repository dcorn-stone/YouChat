#ifndef COMMUNICATE_H
#define COMMUNICATE_H

#define PORT 8080

extern int sock_fd;

int signup(const char *username, const char *nickname, const char *password);

#endif // !COMMUNICATE_H
