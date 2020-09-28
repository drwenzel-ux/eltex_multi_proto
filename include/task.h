#ifndef __TASK_H__
#define __TASK_H__

#include "inet_socket.h"

int tcp_task(int client_fd);
void udp_task(int server_fd);

#endif