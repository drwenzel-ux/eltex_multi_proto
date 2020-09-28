#include "list.h"

struct linked_list *node_create() {
  struct linked_list *n;

  n = mem_calloc(1, sizeof(*n));
  n->data = NULL;
  n->next = NULL;

  return n;
}

void node_push(struct linked_list **root, void *data, size_t len) {
  struct linked_list *new;
  struct linked_list *ptr;
  size_t off;

  new = node_create();

  new->data = mem_calloc(1, len);
  for (off = 0; off < len; off++)
    *((u_char *)(new->data + off)) = *((u_char *)(data + off));

  if ((*root) != NULL) {
    ptr = *root;
    while (ptr->next != NULL)
      ptr = ptr->next;

    ptr->next = new;
  } else {
    (*root) = new;
  }
}

int comparator(void *a, void *b, size_t len) {
  size_t off;
  int ret = 0;

  for (off = 0; off < len; off++) {
    if (*((u_char *)(a + off)) != *((u_char *)(b + off))) {
      ret = -1;
      break;
    }
  }

  return ret;
}

struct linked_list *node_find(struct linked_list **root, void *data,
                              size_t len) {
  struct linked_list *node;
  struct linked_list *found = NULL;

  node = *root;
  for (node = *root; node; node = node->next) {
    if (!comparator(node->data, data, len))
      found = node;
  }

  return found;
}

void node_del(struct linked_list **root, void *data, size_t len) {
  struct linked_list *node;
  struct linked_list *found;

  node = *root;

  if (!comparator(node->data, data, len)) {
    found = node;
    *root = node->next;
  } else {
    while (comparator(node->next->data, data, len))
      node = node->next;

    found = node->next;
    node->next = node->next->next;
  }

  mem_free(found->data);
  mem_free(found);
}

void node_destroy(struct linked_list **root) {
  struct linked_list *node;
  struct linked_list *next;

  node = *root;

  while (node != NULL) {
    next = node->next;
    mem_free(node->data);
    mem_free(node);
    node = next;
  }
}