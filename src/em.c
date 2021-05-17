#include <lyra/em.h>
#include <stdlib.h>
#include <strings.h>
#include <unistd.h>

struct em *em_new() {
  struct em *new = malloc(sizeof(struct em));

  if(new == NULL ) {
    return NULL;
  }

  new->fd = epoll_create1(0);
 
  if(new->fd < 0) {
    free(new);
    return NULL;
  }

  return new;
}

uint8_t em_del(struct em *ctx) {
  close(ctx->fd);
  free(ctx);
  ctx = NULL;
  return 0;
}

uint8_t em_watch(struct em *ctx, int fd, uint8_t events, em_cb cb, void *arg) {
  struct em_curry *curry = malloc(sizeof(struct em_curry));

  if(curry == NULL) {
    return 1;
  }

  curry->fd = fd;
  curry->cb = cb;
  curry->arg = arg;
  curry->buffer = ring_new(4096); /* TODO: make buf size configurable */

  if(curry->buffer == NULL) {
    free(curry);
    return 2;
  }

  curry->event = malloc(sizeof(struct epoll_event));

  if(curry->event == NULL) {
    ring_del(curry->buffer);
    free(curry);
    return 3;
  }

  curry->event->events = 0;

  if(events & EM_READ) {
    curry->event->events |= EPOLLIN;
  } else if(events & EM_WRITE) {
    curry->event->events |= EPOLLOUT;
  }

  curry->event->data.ptr = (void *) curry;

  if(epoll_ctl(ctx->fd, EPOLL_CTL_ADD, fd, curry->event) < 0) {
    ring_del(curry->buffer);
    free(curry->event);
    free(curry);
    return 3;
  } else {
    return 0;
  }
}
