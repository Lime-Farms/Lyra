#ifndef __LYRA_RING_H
#define __LYRA_RING_H

#include <stddef.h>
#include <stdint.h>

#define LYRA_RING 700
#define RING_TOO_SMALL 701
#define RING_EMPTY 702
#define RING_NO_LINE 703

struct ring {
  uint16_t buflen;
  char *buffer;
  char *begin;
  char *end;
  char *reader;
  char *writer;
};

uint16_t ring_new(struct ring *this, uint16_t len);
uint16_t ring_del(struct ring *this);

uint16_t ring_write(struct ring *this, char *str, uint16_t len);
char *ring_read(struct ring *this, uint16_t *len);
char *ring_readln(struct ring *this, uint16_t *len);

#endif
