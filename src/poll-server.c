#include "inet_socket.h"
#include "list.h"
#include "task.h"

#include <poll.h>

volatile sig_atomic_t stop = 0;

void sigint_handler(int sig) { stop = 1; }

int main(int argc, char const *argv[]) {
  int ret;
  int nfds = 1024;
  int tcp_socket;
  int udp_socket;
  struct pollfd reqs[nfds];
  size_t i, size = 2;

  set_handler(sigint_handler, SIGINT, 0);

  tcp_socket = inet_listen("127.0.0.1", 7000, 3);
  udp_socket = inet_bind("127.0.0.1", 7001, SOCK_DGRAM);

  reqs[0].fd = tcp_socket;
  reqs[0].events = POLLIN;

  reqs[1].fd = udp_socket;
  reqs[1].events = POLLIN;

  while (1) {
    ret = poll(reqs, reqs[size - 1].fd + 1, -1);

    if (stop)
      break;

    if (ret == -1) {
      perror("poll failed");
      continue;
    }

    if (reqs[0].revents & POLLIN) {
      int fd = accept(reqs[0].fd, NULL, NULL);
      reqs[size].fd = fd;
      reqs[size++].events = POLLIN;
    }

    if (reqs[1].revents & POLLIN)
      udp_task(reqs[1].fd);

    for (i = 2; i < reqs[size - 1].fd + 1; i++) {
      if (reqs[i].revents & POLLIN) {
        if (tcp_task(reqs[i].fd) == -1) {
          close(reqs[i].fd);
          reqs[i].fd = 0;
          reqs[i].events = 0;
          size--;
        }
      }
    }
  }

  return 0;
}
