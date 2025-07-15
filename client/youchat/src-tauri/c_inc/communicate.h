#ifndef COMMUNICATE_H
#define COMMUNICATE_H

extern int sock_fd;

int signup(const char *username, const char *nickname, const char *password);
int login(const char *username, const char *password);

#endif // !COMMUNICATE_H
