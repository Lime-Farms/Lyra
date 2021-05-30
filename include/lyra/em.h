#ifndef __LYRA_EM_H
#define __LYRA_EM_H

#include <lyra/ring.h>
#include <stdint.h>
#include <sys/epoll.h>

#define EM_READ (1 << 0)
#define EM_WRITE (1 << 1)
#define EM_CLEANUP (1 << 2)

#define EM_MAX_EVENTS 64

struct em_curry;
typedef uint16_t (*em_cb)(struct em_curry *ctx, void *arg);

struct em {
  int fd;
  uint32_t events;
};

struct em_curry {
  int fd;
  struct epoll_event *event;
  em_cb cb;
  void *arg;
  uint8_t options;
  struct em *mgr;
};

struct em *em_new();
uint8_t em_del(struct em *ctx);

uint8_t em_watch(struct em *ctx, int fd, uint8_t events, em_cb cb, void *arg, uint8_t options);
uint8_t em_ignore(struct em *ctx, struct em_curry *curry);
uint8_t em_run(struct em *ctx);

#endif
