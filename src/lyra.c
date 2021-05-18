#define _XOPEN_SOURCE

#include <errno.h>
#include <lyra/hm.h>
#include <lyra/timer.h>
#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <unistd.h>

uint8_t beep(int duration, uint16_t iteration) {
  printf("%d timer beeped!\n", duration);

  if(iteration == 5) {
    return 0;
  } else {
    return 1;
  }
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
  struct timer *t1 = timer_new(beep, 10);
  struct timer *t2 = timer_new(beep, 5);
  timer_start(mgr, t1);
  timer_start(mgr, t2);
  em_run(mgr);
  em_del(mgr);
  hm_del(&env_vars);
  return 0;
}
