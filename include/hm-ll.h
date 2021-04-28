#ifndef __LYRA_HM_LL_H
#define __LYRA_HM_LL_H

#include <hm-node.h>

struct hm_ll {
  struct hm_node *list;
  struct hm_node *head;
  struct hm_node *tail;
};

struct hm_ll hm_ll_new();
uint8_t hm_ll_del(struct hm_ll *list);

struct hm_ll *hm_ll_add(struct hm_ll *list, const char *key, const char *value);
uint8_t hm_ll_rem(struct hm_ll *list, struct hm_node *entry);

#endif
