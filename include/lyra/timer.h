#ifndef __LYRA_TIMER_H
#define __LYRA_TIMER_H

#include <lyra/em.h>
#include <sys/timerfd.h>

typedef uint8_t (*timer_cb)(int duration, uint16_t iteration);

struct timer {
  int fd;
  struct itimerspec stop;
  int duration;
  uint16_t iteration;
  timer_cb cb;
};

struct timer *timer_new(timer_cb cb, int duration);
uint8_t timer_del(struct timer *tmr);

uint8_t timer_start(struct em *mgr, struct timer *tmr);
uint8_t timer_stop(struct em *mgr, struct timer *tmr);

#endif
