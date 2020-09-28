#include "socket.h"

int socket_create(int family, int type, int protocol) {
  int fd;

  fd = socket(family, type, protocol);
  if (fd == -1)
    perror_die("socket_create: socket return -1", 1);

  return fd;
}

void socket_bind(int fd, const struct sockaddr *sa, socklen_t len) {
  int ret;

  ret = bind(fd, sa, len);
  if (ret == -1) {
    close(fd);
    perror_die("socket_bind: bind -1", 1);
  }
}

void socket_listen(int fd, int backlog) {
  int ret;

  ret = listen(fd, backlog);
  if (ret == -1) {
    close(fd);
    perror_die("socket_listen: listen -1", 1);
  }
}

void socket_connect(int fd, const struct sockaddr *sa, socklen_t len) {
  int ret;

  ret = connect(fd, sa, len);
  if (ret == -1) {
    close(fd);
    perror_die("connect: connect return -1", 1);
  }
}

void set_sockaddr(struct sockaddr *sa, int fa, const char *addr,
                  in_addr_t port) {
  struct sockaddr_in *ptr;

  ptr = (struct sockaddr_in *)sa;
  ptr->sin_family = fa;
  ptr->sin_addr.s_addr = inet_addr(addr);
  ptr->sin_port = htons(port);
}