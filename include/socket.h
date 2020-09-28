#ifndef __SOCKET_H__
#define __SOCKET_H__

#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>

#include "utils.h"

int socket_create(int family, int type, int protocol);
void socket_bind(int fd, const struct sockaddr *sa, socklen_t len);
void socket_listen(int fd, int backlog);
void socket_connect(int fd, const struct sockaddr *sa, socklen_t len);
void set_sockaddr(struct sockaddr *sa, int fa, const char *addr,
                  in_addr_t port);

#endif