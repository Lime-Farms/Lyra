#define _POSIX_C_SOURCE 199309L

#include <errno.h>
#include <lyra/timer.h>
#include <stdlib.h>
#include <unistd.h>

static uint8_t timer_beep(struct em_curry *ctx, void *arg) {
  struct timer *this = arg;
  uint64_t buf = 0;
  ssize_t bytes = read(ctx->fd, &buf, sizeof(buf));

  if(bytes < 0 && errno != EAGAIN) {
    return 2;
  } else if(bytes == sizeof(uint64_t)) {
    this->iteration += 1;
    uint8_t res = this->cb(this->duration, this->iteration);

    if(res) {
      this->stop.it_value.tv_sec = this->duration;

      if(timerfd_settime(this->fd, 0, &this->stop, NULL) < 0) {
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

uint16_t timer_new(struct timer *this, int duration, timer_cb cb) {
  this->fd = timerfd_create(CLOCK_REALTIME, TFD_NONBLOCK);

  if(this->fd < 0) {
    return TIMER_INST_FAIL;
  }

  this->stop.it_interval.tv_sec = 0;
  this->stop.it_interval.tv_nsec = 0;
  this->stop.it_value.tv_sec = duration;
  this->stop.it_valur.tv_nsec = 0;
  this->duration = duration;
  this->iteration = 0;
  this->cb = cb;
  return LYRA_SUCCESS;
}

uint16_t timer_del(struct timer *this) {
  close(this->fd);
  this->fd = -1;
  this->stop.it_interval.tv_sec = 0;
  this->stop.it_interval.tv_nsec = 0;
  this->stop.it_value.tv_sec = 0;
  this->stop.it_value.tv_nsec = 0;
  this->duration = 0;
  this->iteration = 0;
  return LYRA_SUCCESS;
}

uint8_t timer_start(struct timer *this, struct em *mgr) {
  this->stop.it_value.tv_sec = this->duration;

  if(timerfd_settime(this->fd, 0, &this->stop, NULL) < 0) {
    return 42;
  }

  return em_watch(mgr, this->fd, EM_READ, timer_beep, this, 0);
}

uint8_t timer_stop(struct timer *this) {
  this->stop.it_value.tv_sec = 0;

  if(timerfd_settime(this->fd, 0, &this->stop, NULL) < 0) {
    return 1;
  } else {
    return 0;
  }
}
