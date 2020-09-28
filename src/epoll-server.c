#include "inet_socket.h"
#include "task.h"

#include <fcntl.h>
#include <sys/epoll.h>

volatile sig_atomic_t stop = 0;

void sigint_handler(int sig) { stop = 1; }

void setnonblocking(int sockfd) {
  int flags = fcntl(sockfd, F_GETFL, 0);
  if (flags == -1) {
    perror_die("fcntl F_GETFL", 1);
  }

  if (fcntl(sockfd, F_SETFL, flags | O_NONBLOCK) == -1) {
    perror_die("fcntl F_SETFL O_NONBLOCK", 1);
  }
}

int main(int argc, char const *argv[]) {
  int epfd, nfds;
  size_t i, size = 10;
  int tcp_socket, udp_socket;
  struct epoll_event evs[size];
  struct epoll_event tcp_ev, udp_ev;

  tcp_socket = inet_listen("127.0.0.1", 3000, 3);
  udp_socket = inet_bind("127.0.0.1", 3001, SOCK_DGRAM);

  epfd = epoll_create(2);
  if (epfd == -1)
    perror_die("epoll_create failed!", 1);

  tcp_ev.data.fd = tcp_socket;
  tcp_ev.events = EPOLLIN;

  if (epoll_ctl(epfd, EPOLL_CTL_ADD, tcp_socket, &tcp_ev) == -1)
    perror_die("epoll_ctl add tcp socket", 1);

  udp_ev.data.fd = udp_socket;
  udp_ev.events = EPOLLIN;

  if (epoll_ctl(epfd, EPOLL_CTL_ADD, udp_socket, &udp_ev) == -1)
    perror_die("epoll_ctl add udp socket", 1);

  for (;;) {
    nfds = epoll_wait(epfd, evs, size, -1);

    if (stop)
      break;

    if (nfds == -1) {
      perror("epoll_wait return -1");
      continue;
    }

    for (i = 0; i < size; i++) {
      /* tcp socket */
      if (evs[i].data.fd == tcp_socket) {
        printf("events.data[i].fd %d\n", evs[i].data.fd);

        struct epoll_event ev;
        ev.data.fd = accept(evs[i].data.fd, NULL, NULL);
        ev.events = EPOLLIN | EPOLLET;

        setnonblocking(ev.data.fd);
        if (epoll_ctl(epfd, EPOLL_CTL_ADD, ev.data.fd, &ev) == -1)
          perror("epoll_ctl add");
      }
      /* udp socket */
      else if (evs[i].data.fd == udp_socket) {
        udp_task(udp_socket);
      }
      /* other fds */
      else if (evs[i].events & EPOLLIN) {
        if (tcp_task(evs[i].data.fd) == -1) {
          if (epoll_ctl(epfd, EPOLL_CTL_DEL, evs[i].data.fd, &evs[i]) == -1)
            perror("epoll_ctl add udp socket");
          close(evs[i].data.fd);
        }
      }
    }
  }

  close(epfd);
  return 0;
}