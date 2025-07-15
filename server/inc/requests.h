#ifndef REQUESTS_H
#define REQUESTS_H

#include "jansson.h"

const char *request_handler(char *buffer);
extern int cus_write(int fd, const char *message);
extern const char *cus_read(int fd);

#endif // REQUESTS_H
