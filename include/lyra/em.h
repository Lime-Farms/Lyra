#ifndef __LYRA_EM_H
#define __LYRA_EM_H

#include <lyra/ring.h>
#include <stdint.h>
#include <sys/epoll.h>

#define EM_READ (1 << 0)
#define EM_WRITE (1 << 1)

#define EM_MAX_EVENTS 64

typedef uint8_t (*em_cb)(struct epoll_event *event, void *arg);

struct em_curry {
  int fd;
  struct epoll_event *event;
  em_cb cb;
  void *arg;
  struct ring *buffer;
};

struct em {
  int fd;
};

struct em *em_new();
uint8_t em_del(struct em *ctx);

#endif
