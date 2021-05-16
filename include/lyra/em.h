#ifndef __LYRA_EM_H
#define __LYRA_EM_H

#include <stdint.h>
#include <sys/epoll.h>

#define EM_READ (1 << 0)
#define EM_WRITE (1 << 1)

#define EM_MAX_EVENTS 64

typedef uint8_t (*em_cb)(struct epoll_event *event, void *arg);

struct em_cooked_curry {
  em_cb cb;
  void *arg;
};

struct em {
  int fd;
};

struct em *em_new();
uint8_t em_del(struct em *ctx);

#endif


/*
#define _POSIX_C_SOURCE 199309L
#include <sys/epoll.h>
#include <sys/timerfd.h>
*/


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
