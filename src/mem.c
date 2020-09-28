#include "mem.h"

void *mem_calloc(size_t n, size_t s) {
  void *x;

  if (n * s == 0)
    perror_die("mem_calloc: before calloc", 1);

  x = calloc(n, s);
  if (!x)
    perror_die("mem_calloc: calloc failed", 1);

  return x;
}

void *mem_realloc(void *ptr, size_t size) {
  void *p;

  if (ptr == NULL || size == 0)
    perror_die("mem_realloc: before realloc", 1);

  p = realloc(ptr, size);
  if (!p)
    perror_die("mem_realloc: realloc failed", 1);

  return p;
}

void mem_free(void *ptr) {
  if (ptr == NULL)
    perror_die("mem_free: free failed", 1);

  free(ptr);
}