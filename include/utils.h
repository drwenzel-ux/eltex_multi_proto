#ifndef __UTILS_H__
#define __UTILS_H__

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "mem.h"

void perror_die(const char *m, int ret);
void set_handler(void *handler, int signum, int flags);

#endif