#include "inet_socket.h"
#include "list.h"
#include "task.h"

#include <sys/select.h>

volatile sig_atomic_t stop = 0;

void sigint_handler(int sig) { stop = 1; }

int main(int argc, char const *argv[]) {
  int ret;
  int nfds;
  int tcp_socket;
  int udp_socket;
  int current_fd;
  fd_set rfds, afds;
  struct linked_list *list;
  struct linked_list *node;

  list = NULL;

  set_handler(sigint_handler, SIGINT, 0);

  tcp_socket = inet_listen("127.0.0.1", 7000, 3);
  udp_socket = inet_bind("127.0.0.1", 7001, SOCK_DGRAM);

  nfds = 1024;

  FD_ZERO(&afds);
  FD_SET(tcp_socket, &afds);
  FD_SET(udp_socket, &afds);

  while (1) {
    memcpy(&rfds, &afds, sizeof(rfds));

    ret = select(nfds, &rfds, NULL, NULL, NULL);

    if (stop)
      break;

    if (ret == -1) {
      perror("select return -1");
      continue;
    }

    if (FD_ISSET(tcp_socket, &rfds)) {
      int new_fd = inet_accept(tcp_socket);
      node_push(&list, &new_fd, sizeof(int));
      FD_SET(new_fd, &afds);
    }

    if (FD_ISSET(udp_socket, &rfds))
      udp_task(udp_socket);

    for (node = list; node; node = node->next) {
      current_fd = *(int *)node->data;
      if (FD_ISSET(current_fd, &rfds)) {
        if (tcp_task(current_fd) == -1) {
          node_del(&list, &current_fd, sizeof(int));
          FD_CLR(current_fd, &afds);
          close(current_fd);
          break;
        }
      }
    }
  }

  close(udp_socket);
  close(tcp_socket);

  node_destroy(&list);
  return 0;
}
