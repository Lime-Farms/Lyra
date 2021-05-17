#ifndef __LYRA_EM_H
#define __LYRA_EM_H

#include <lyra/curry.h>
#include <lyra/ring.h>
#include <stdint.h>

#define EM_READ (1 << 0)
#define EM_WRITE (1 << 1)

#define EM_MAX_EVENTS 64

struct em {
  int fd;
};

struct em *em_new();
uint8_t em_del(struct em *ctx);

#endif
