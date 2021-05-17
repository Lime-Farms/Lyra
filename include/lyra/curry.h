#ifndef __LYRA_CURRY_H
#define __LYRA_CURRY_H

#include <lyra/ring.h>
#include <sys/epoll.h>

struct em_curry;
typedef uint8_t (*em_cb)(struct em_curry *ctx, void *arg);

struct em_curry {
  int fd;
  struct epoll_event *event;
  em_cb cb;
  void *arg;
  struct ring *buffer;
};

#endif
