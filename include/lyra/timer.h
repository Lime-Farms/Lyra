#ifndef __LYRA_TIMER_H
#define __LYRA_TIMER_H

#include <lyra/em.h>
#include <sys/timerfd.h>

#define LYRA_TIMER 600
#define TIMER_INST_FAIL 601
#define TIMER_NOT_READY 602
#define TIMER_RENEW_FAIL 603
#define TIMER_START_FAIL 604
#define TIMER_STOP_FAIL 605

typedef uint8_t (*timer_cb)(int duration, uint16_t iteration);

struct timer {
  int fd;
  struct itimerspec stop;
  int duration;
  uint16_t iteration;
  timer_cb cb;
};

uint16_t timer_new(struct timer *this, int duration, timer_cb cb);
uint16_t timer_del(struct timer *this);

uint16_t timer_start(struct timer *this, struct em *mgr);
uint16_t timer_stop(struct timer *this);

#endif
