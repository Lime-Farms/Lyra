#ifndef __LYRA_HM_NODE_H
#define __LYRA_HM_NODE_H

#include <stdint.h>

struct hm_node {
  struct hm_node *next;
  struct hm_node *prev;
  const char *key;
  const void *value;
};

struct hm_node *hm_node_new(const char *key, const void *value);
uint8_t hm_node_del(struct hm_node *entry);

struct hm_node *hm_node_add(struct hm_node *entry, const char *key, const void *value);

#endif
