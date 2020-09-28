#ifndef __INET_SOCKET_H__
#define __INET_SOCKET_H__

#include "socket.h"

#define FAMILY AF_INET

int inet_connect(const char *addr, in_port_t port, int type);
int inet_bind(const char *addr, in_port_t port, int type);
int inet_listen(const char *addr, in_port_t port, int backlog);
int inet_accept(int fd);

#endif