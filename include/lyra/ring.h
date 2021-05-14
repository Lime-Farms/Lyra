#ifndef __LYRA_RING_H
#define __LYRA_RING_H

#include <stddef.h>
#include <stdint.h>

struct ring {
  uint16_t buflen;
  char *buffer;
  char *begin;
  char *end;
  char *reader;
  char *writer;
};

struct ring *ring_new(uint16_t buflen);
uint8_t ring_del(struct ring *buf);

uint16_t ring_write(struct ring *buf, char *str, uint16_t orig_len);
char *ring_read(struct ring *buf, uint16_t *len);
char *ring_readln(struct ring *buf, uint16_t *len);

#endif
