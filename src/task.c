#include "task.h"

void udp_task(int server_fd) {
  char udp_buffer[10];
  memset(udp_buffer, 0, 10);

  struct sockaddr st;
  socklen_t st_size = sizeof(st);
  recvfrom(server_fd, udp_buffer, 10, 0, &st, &st_size);
  sendto(server_fd, udp_buffer, 10, 0, &st, st_size);
}

int tcp_task(int client_fd) {
  printf("client_fd == %d\n", client_fd);

  char tcp_buffer[10];
  memset(tcp_buffer, 0, sizeof(tcp_buffer));
  read(client_fd, tcp_buffer, sizeof(tcp_buffer));

  if (strncmp(tcp_buffer, ":exit", 5) == 0) {
    puts("disconnect!");
    return -1;
  }

  write(client_fd, tcp_buffer, sizeof(tcp_buffer));
  return 0;
}