#include "inet_socket.h"

int inet_accept(int fd) {
  int client_fd;
  struct sockaddr_in sa;
  socklen_t len;

  len = sizeof(sa);
  client_fd = accept(fd, (struct sockaddr *)&sa, &len);

  if (client_fd != -1)
    printf("accept from %s:%d\n", inet_ntoa(sa.sin_addr), ntohs(sa.sin_port));

  return client_fd;
}

int inet_connect(const char *addr, in_port_t port, int type) {
  int fd;
  struct sockaddr sa;

  fd = socket_create(FAMILY, type, 0);
  set_sockaddr(&sa, FAMILY, addr, port);
  socket_connect(fd, &sa, sizeof(sa));

  return fd;
}

static int inet_passive_socket(const char *addr, in_port_t port, int type,
                               int backlog, int do_listen) {
  int fd, ret;
  int optval = 1;
  socklen_t s;
  struct sockaddr sa;

  fd = socket_create(FAMILY, type, 0);

  if (do_listen) {
    ret = setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));
    if (ret == -1) {
      close(fd);
      perror_die("inet_passive_socket: setsockopt -1", 1);
    }
  }

  set_sockaddr(&sa, FAMILY, addr, port);
  s = sizeof(sa);

  socket_bind(fd, &sa, s);

  if (do_listen)
    socket_listen(fd, backlog);

  return fd;
}

int inet_listen(const char *addr, in_port_t port, int backlog) {
  return inet_passive_socket(addr, port, SOCK_STREAM, backlog, 1);
}

int inet_bind(const char *addr, in_port_t port, int type) {
  return inet_passive_socket(addr, port, type, 0, 0);
}