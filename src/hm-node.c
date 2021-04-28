#include <hm-node.h>
#include <stdlib.h>

struct hm_node *hm_node_new(const char *key, const void *value) {
  struct hm_node *new = calloc(1, sizeof(struct hm_node));
  new->next = NULL;
  new->prev = NULL;
  new->key = key;
  new->value = value;
  return new;
}

uint8_t hm_node_del(struct hm_node *entry) {
  if(entry == NULL) {
    return 1;
  } else {
    free(entry);
    return 0;
  }
}

struct hm_node *hm_node_add(struct hm_node *entry, const char *key, const void *value) {
  struct hm_node *new = hm_node_new(key, value);
  new->prev = entry;

  if(entry != NULL) {
    new->next = entry->next;
    entry->next = new;
  }

  return new;
}
