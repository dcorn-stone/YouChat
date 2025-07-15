#ifndef LIBCLIENT_H
#define LIBCLIENT_H

extern int cus_write(int fd, const char *message);
extern const char *cus_read(int fd);

int client_connect(const char *address, int port);
void client_close();

#endif // !LIBCLIENT_H
