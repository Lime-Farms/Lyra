#include <lyra/timer.h>
#include <stdlib.h>

static uint8_t timer_beep(struct em_curry *ctx, void *arg) {
  struct timer *tmr = arg;
  uint64_t buf = 0;
  ssize_t bytes = read(ctx->fd, &buf, sizeof(buf));

  if(bytes < 0 && errno != EAGAIN) {
    return 2;
  } else if(bytes == sizeof(uint64_t)) {
    tmr->iteration += 1;
    uint8_t res = tmr->cb(tmr->duration, tmr->iteration);

    if(res) {
      tmr->stop.it_value.tv_sec = tmr->duration;

      if(timerfd_settime(tmr->fd, 0, tmr->stop, NULL) < 0) {
        return 1;
      } else {
        return 0;
      }
    } else {
      em_ignore(ctx->mgr, ctx);
      return 0;
    }
  }

  return 3;
}

struct timer *timer_new(timer_cb cb, int duration) {
  struct timer *new = malloc(sizeof(struct timer));

  if(new == NULL) {
    return NULL;
  }

  new->fd = timerfd_create(CLOCK_REALTIME, TFD_NONBLOCK);

  if(new->fd < 0) {
    free(new);
    return NULL;
  }

  new->stop = {{ 0, 0 }, { duration, 0 }};
  new->duration = duration;
  new->iteration = 0;
  new->cb = cb;
  return new;
}

uint8_t timer_del(struct timer *tmr) {
  close(tmr->fd);
  free(tmr);
  return 0;
}

uint8_t timer_start(struct em *mgr, struct timer *tmr) {
  tmr->stop.it_value.tv_sec = tmr->duration;

  if(timerfd_settime(tmr->fd, 0, tmr->stop, NULL) < 0) {
    return 42;
  }

  return em_watch(mgr, tmr->fd, EM_READ, timer_beep, tmr, 0);
}

uint8_t timer_stop(struct timer *tmr) {
  tmr->stop.it_value.tv_sec = 0;

  if(timerfd_settime(tmr->fd, 0, tmr->stop, NULL) < 0) {
    return 1;
  } else {
    return 0;
  }
}
