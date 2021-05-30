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

  new->events = 0;
  return new;
}

uint8_t em_del(struct em *ctx) {
  close(ctx->fd);
  free(ctx);
  ctx = NULL;
  return 0;
}

uint8_t em_watch(struct em *ctx, int fd, uint8_t events, em_cb cb, void *arg, uint8_t options) {
  struct em_curry *curry = malloc(sizeof(struct em_curry));

  if(curry == NULL) {
    return 1;
  }

  curry->fd = fd;
  curry->cb = cb;
  curry->arg = arg;
  curry->mgr = ctx;
  curry->options = options;
  ctx->events += 1;

  curry->event = malloc(sizeof(struct epoll_event));

  if(curry->event == NULL) {
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
    free(curry->event);
    free(curry);
    return 3;
  } else {
    return 0;
  }
}

uint8_t em_ignore(struct em *ctx, struct em_curry *curry) {
  if(epoll_ctl(ctx->fd, EPOLL_CTL_DEL, curry->fd, NULL) < 0) {
    return 1;
  }

  ctx->events -= 1;

  if(curry->options & EM_CLEANUP) {
    free(curry->arg);
    close(curry->fd);
  }

  free(curry->event);
  free(curry);
  return 0;
}

uint8_t em_run(struct em *ctx) {
  while(ctx->events > 0) {
    struct epoll_event events[EM_MAX_EVENTS] = { 0 };
    int max_events = epoll_wait(ctx->fd, events, EM_MAX_EVENTS, 0);
    int idx = 0;

    for(; idx < max_events; idx += 1) {
      struct em_curry *curry = events[idx].data.ptr;
      curry->cb(curry, curry->arg);
    }
  }

  return 0;
}
