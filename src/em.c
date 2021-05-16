#include <lyra/em.h>
#include <stdlib.h>
#include <unistd.h>

struct em *em_new() {
  struct em *new = malloc(sizeof(struct em));
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

int em_watch(struct em *ctx, int fd, uint8_t events, em_cb cb, void *arg) {
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

  struct em_cooked_curry *curry = malloc(sizeof(struct em_cooked_curry));

  if(curry == NULL) {
    free(event);
    return 2;
  }

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


/*
 
  struct itimerspec stop = {{ 0, 0 }, { 5, 0 }};
  int timer = timerfd_create(CLOCK_REALTIME, TFD_NONBLOCK);

  if(timer < 0) {
    fprintf(stderr, "%s:%d: error: (%d) %s\n", __FILE__, __LINE__, errno, strerror(errno));
    return 1;
  }

  struct epoll_event event = { EPOLLIN, { .fd = timer }};

  if(epoll_ctl(mgr, EPOLL_CTL_ADD, timer, &event) < 0) {
    fprintf(stderr, "%s:%d: error: (%d) %s\n", __FILE__, __LINE__, errno, strerror(errno));
    return 1;
  }

  if(timerfd_settime(timer, 0, &stop, NULL) < 0) {
    fprintf(stderr, "%s:%d: error: (%d) %s\n", __FILE__, __LINE__, errno, strerror(errno));
    return 1;
  }

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