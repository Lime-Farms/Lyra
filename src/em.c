#include <lyra/em.h>
#include <stdlib.h>
#include <unistd.h>

/*
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
*/

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

  new->event_idx = 0;

  for(; new->event_idx < EM_MAX_EVENTS; new->event_idx += 1) {
    new->events[new->event_idx] = malloc(sizeof(struct epoll_event));

    if(new->events[new->event_idx] == NULL) {
      close(new->fd);
      free(new);
      return NULL;
    }

    new->free_events[new->event_idx] = new->event_idx;
  }

  return new;
}

uint8_t em_del(struct em *ctx) {
  uint8_t idx = 0;

  for(; idx < EM_MAX_EVENTS; idx += 1) {
    if(ctx->events[idx] != NULL) {
      free(ctx->events[idx]);
    }
  }

  close(ctx->fd);
  free(ctx);
  ctx = NULL;
  return 0;
}

int em_watch(struct em *ctx, int fd, uint8_t events, em_cb cb, void *arg) {
  /*
   * have em_watch throw the fd into a backlog if its full
   */

  struct epoll_event *event = malloc(sizeof(struct epoll_event));

  if(event == NULL) {
    return 1;
  }

  event->events = 0;

  if(events & EM_READ) {
    event->events |= EPOLLIN;
  } else if(events & EM_WRITE) {
    event->events |= EPOLLOUT;
  }

  struct em_curry *curry = malloc(sizeof(struct em_curry));

  if(curry == NULL) {
    free(event);
    return 2;
  }

  curry->fd = fd;
  curry->cb = cb;
  curry->arg = arg;
  event->data.ptr = (void *) curry;

  if(epoll_ctl(ctx->fd, EPOLL_CTL_ADD, fd, event) < 0) {
    free(curry);
    free(event);
    return 3;
  } else {
    return 0;
  }
}

int em_stop(struct em *ctx, int fd)

/*
  int running = 42;

  while(running) {
    struct epoll_event events[LYRA_MAX_EVENTS] = { 0 };
    int max_events = epoll_wait(mgr, events, LYRA_MAX_EVENTS, 0);
    int count = 0;

    for(; count < max_events; count += 1) {
      if()
    }


    uint64_t buf = 0;
    ssize_t bytes = read(timer, &buf, sizeof(buf));

    if(bytes < 0 && errno != EAGAIN) {
      fprintf(stderr, "%s:%d: error: (%d) %s\n", __FILE__, __LINE__, errno, strerror(errno));
      running = 0;
    } else if(bytes == sizeof(uint64_t)) {
      running = 0;
    }
  }

  close(timer);
  */

/*
  struct epoll_event event, events[MAX_EVENTS];
 
  while(running)
  {
    printf("\nPolling for input...\n");
    event_count = epoll_wait(epoll_fd, events, MAX_EVENTS, 30000);
    printf("%d ready events\n", event_count);
    for(i = 0; i < event_count; i++)
    {
      printf("Reading file descriptor '%d' -- ", events[i].data.fd);
      bytes_read = read(events[i].data.fd, read_buffer, READ_SIZE);
      printf("%zd bytes read.\n", bytes_read);
      read_buffer[bytes_read] = '\0';
      printf("Read '%s'\n", read_buffer);
 
      if(!strncmp(read_buffer, "stop\n", 5))
        running = 0;
    }
  }
*/