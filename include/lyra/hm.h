#ifndef __LYRA_HM_H
#define __LYRA_HM_H

#include <lyra/hm-ll.h>

#define HM_OVERWRITE (1 << 0)
#define HM_BKT_SIZE 64

typedef uint8_t (*hm_cb)(const char *key, const void *value);

struct hm {
  struct hm_ll bkts[HM_BKT_SIZE];
  uint8_t options;
};

struct hm hm_new(uint8_t options);
uint8_t hm_del(struct hm *map);

uint8_t hm_add(struct hm *map, const char *key, const void *value);
uint8_t hm_has(struct hm *map, const char *key);
const void *hm_key(struct hm *map, const char *key);
const void *hm_key_default(struct hm *map, const char *key, const void *value);
const void *hm_rem(struct hm *map, const char *key);
void hm_foreach(struct hm *map, hm_cb cb);

#endif
