#ifndef __MEM_H__
#define __MEM_H__

#include "utils.h"
#include <stddef.h>

void *mem_calloc(size_t n, size_t s);
void *mem_realloc(void *ptr, size_t size);
void mem_free(void *ptr);

#endif