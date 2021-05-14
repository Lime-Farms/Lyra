#include <hm-ll.h>
#include <stdlib.h>


struct hm_ll hm_ll_new() {
  struct hm_ll new;
  new.list = NULL;
  new.head = NULL;
  new.tail = NULL;
  return new;
}

uint8_t hm_ll_del(struct hm_ll *list) {
  struct hm_node *iter = list->head;

  for(; iter != NULL; iter = iter->next) {
    hm_ll_rem(list, iter);
  }

  return 0;
}

struct hm_ll *hm_ll_add(struct hm_ll *list, const char *key, const void *value) {
  list->list = hm_node_add(list->tail, key, value);
  list->tail = list->list;

  if(list->head == NULL) {
    list->head = list->list;
  }

  return 0;
}

uint8_t hm_ll_rem(struct hm_ll *list, struct hm_node *entry) {
  if(entry == list->head && entry == list->tail) {
    list->head = NULL;
    list->tail = NULL;
  } else if(entry == list->head) {
    list->head = list->head->next;
    list->head->prev = NULL;
  } else if(entry == list->tail) {
    list->tail = list->tail->prev;
    list->tail->next = NULL;
  } else {
    entry->prev->next = entry->next;
    entry->next->prev = entry->prev;
  }

  entry->next = NULL;
  entry->prev = NULL;
  entry->key = NULL;
  entry->value = NULL;
  free(entry);
  entry = NULL;
  return 0;
}
