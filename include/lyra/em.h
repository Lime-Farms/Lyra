#ifndef __LYRA_EM_H
#define __LYRA_EM_H

#include <stdint.h>
#include <sys/epoll.h>

#define EM_READ (1 << 0)
#define EM_WRITE (1 << 1)

#define EM_MAX_EVENTS 64

typedef uint8_t (*em_cb)(struct epoll_event *event, void *arg);

struct em_curry {
  em_cb cb;
  void *arg;
  int fd;
};

struct em {
  int fd;
  struct epoll_event *events[EM_MAX_EVENTS];
  uint8_t free_events[EM_MAX_EVENTS];
  uint8_t event_idx;
};

struct em *em_new();
uint8_t em_del(struct em *ctx);

int em_watch(struct em *ctx, int fd, uint8_t events, em_cb cb, void *arg);

#endif
