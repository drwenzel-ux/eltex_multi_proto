#include "utils.h"

void perror_die(const char *m, int ret) {
  perror(m);
  exit(0);
}

void set_handler(void *handler, int signum, int flags) {
  struct sigaction sa;
  sa.sa_handler = handler;
  sa.sa_flags = flags;
  sigemptyset(&sa.sa_mask);
  sigaction(signum, &sa, 0);
}