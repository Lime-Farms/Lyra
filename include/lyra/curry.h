#ifndef __LYRA_CURRY_H
#define __LYRA_CURRY_H

#include <sys/epoll.h>

struct em_curry {
  int fd;
  struct epoll_event *event;
  em_cb cb;
  void *arg;
  struct ring *buffer;
};

#endif
