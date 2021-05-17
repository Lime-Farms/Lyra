#define _XOPEN_SOURCE
#define _POSIX_C_SOURCE 199309L

/* TODO: determine which headers are needed after the test is finished */

#include <errno.h>
#include <lyra/em.h>
#include <lyra/hm.h>
#include <stdio.h>
#include <strings.h>
#include <unistd.h>

#include <sys/timerfd.h>
#include <time.h>
#include <string.h>

uint8_t beep(struct em_curry *ctx, void *arg) {
  uint64_t buf = 0;
  ssize_t bytes = read(ctx->fd, &buf, sizeof(buf));

  if(bytes < 0 && errno != EAGAIN) {
    fprintf(stderr, "%s:%d: error: (%d) %s\n", __FILE__, __LINE__, errno, strerror(errno));
  } else if(bytes == sizeof(uint64_t)) {
    printf("beep!\n");
    exit(0);
  }

  return 0;
}

int main(int argc, char **argv, char **env) {
  int arg = opterr = 0;

  while((arg = getopt(argc, argv, "")) > 0) {
    if(arg == '?') {
      fprintf(stderr, "error: option '-%c' incorrectly used.\n", optopt);
      return EINVAL;
    }
  }

  char **var = env;
  struct hm env_vars = hm_new(0);

  for(; *var != NULL; var += 1) {
    char *const place = index(*var, '=');

    if(place == NULL) { /* Is this even possible? */
      hm_add(&env_vars, *var, "");
    } else {
      *place = '\0';
      hm_add(&env_vars, *var, place + 1);
    }
  }

  struct em *mgr = em_new();
  int duration = 10;
  struct itimerspec stop = {{ 0, 0 }, { duration, 0 }};
  int timer = timerfd_create(CLOCK_REALTIME, TFD_NONBLOCK);

  if(em_watch(mgr, timer, EM_READ, beep, &duration) > 0) {
    em_del(mgr);
    hm_del(&env_vars);
    fprintf(stderr, "something went wrong :(\n");
  }

  if(timerfd_settime(timer, 0, &stop, NULL) < 0) {
    fprintf(stderr, "%s:%d: error: (%d) %s\n", __FILE__, __LINE__, errno, strerror(errno));
    return 1;
  }

  int running = 42;

  while(running) {
    struct epoll_event events[EM_MAX_EVENTS] = { 0 };
    int max_events = epoll_wait(mgr->fd, events, EM_MAX_EVENTS, 0);
    int idx = 0;

    for(; idx < max_events; idx += 1) {
      struct em_curry *curry = events[idx].data.ptr;
      curry->cb(curry, curry->arg);
    }
  }

  close(timer);

  em_del(mgr);
  hm_del(&env_vars);
  return 0;
}
