#include <lyra/hm.h>
#include <string.h>

struct hm hm_new(uint8_t options) {
  struct hm new;
  new.options = options;
  uint8_t bkt = 0;

  for(; bkt < HM_BKT_SIZE; bkt += 1) {
    new.bkts[bkt] = hm_ll_new();
  }

  return new;
}

uint8_t hm_del(struct hm *map) {
  uint8_t bkt = 0;

  for(; bkt < HM_BKT_SIZE; bkt += 1) {
    hm_ll_del(&map->bkts[bkt]);
  }

  return 0;
}

static uint16_t hm_hash(const char *str) {
  uint16_t value = 0;
  uint16_t high = 0;

  for(; *str; str += 1) {
    value = (value << 4) + *str;

    if((high = value & 0xF0000000) > 0) {
      value ^= high >> 24;
    }

    value &= ~high;
  }

  return value % HM_BKT_SIZE;
}

uint8_t hm_add(struct hm *map, const char *key, const void *value) {
  if(key == NULL || value == NULL) {
    return 1;
  }

  int idx = hm_hash(key);
  struct hm_node *iter = map->bkts[idx].head;

  for(; iter != NULL; iter = iter->next) {
    if(strcmp(iter->key, key) == 0) {
      if(map->options & HM_OVERWRITE) {
        iter->value = value;
        return 0;
      } else {
        return 1;
      }
    }
  }

  hm_ll_add(&map->bkts[idx], key, value);
  return 0;
}

uint8_t hm_has(struct hm *map, const char *key) {
  return hm_key(map, key) != NULL;
}

const void *hm_key(struct hm *map, const char *key) {
  return hm_key_default(map, key, NULL);
}

const void *hm_key_default(struct hm *map, const char *key, const void *value) {
  if(key == NULL) {
    return NULL;
  }

  int idx = hm_hash(key);
  struct hm_node *iter = map->bkts[idx].head;

  for(; iter != NULL; iter = iter->next) {
    if(strcmp(iter->key, key) == 0) {
      return iter->value;
    }
  }

  return value;
}

const void *hm_rem(struct hm *map, const char *key) {
  if(key == NULL) {
    return NULL;
  }

  int idx = hm_hash(key);
  struct hm_node *iter = map->bkts[idx].head;

  for(; iter != NULL; iter = iter->next) {
    if(strcmp(iter->key, key) == 0) {
      const void *value = iter->value;
      hm_ll_rem(&map->bkts[idx], iter);
      return value;
    }
  }

  return NULL;
}

void hm_foreach(struct hm *map, hm_cb cb) {
  int count = 0;

  for(; count < HM_BKT_SIZE; count += 1) {
    struct hm_node *iter = map->bkts[count].head;

    for(; iter != NULL; iter = iter->next) {
      cb(iter->key, iter->value);
    }
  }
}
