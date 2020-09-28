#ifndef __LIST_H__
#define __LIST_H__

#include "mem.h"

struct linked_list {
  void *data;
  struct linked_list *next;
};

int comparator(void *a, void *b, size_t len);

struct linked_list *node_create();
void node_push(struct linked_list **root, void *data, size_t len);
void node_del(struct linked_list **root, void *data, size_t len);
void node_destroy(struct linked_list **root);
struct linked_list *node_find(struct linked_list **root, void *data,
                              size_t len);

#endif