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
  struct timer t1;
  struct timer t2;

  if(timer_new(&t1, 10, beep) > 0) {
    fprintf(stderr, "uh oh: t1\n");
    em_del(mgr);
    hm_del(&env_vars);
    return 1;
  } else if(timer_new(&t2, 5, beep) > 0) {
    fprintf(stderr, "uh oh: t2\n");
    timer_del(&t1);
    em_del(mgr);
    hm_del(&env_vars);
    return 1;
  }

  timer_start(&t1, mgr);
  timer_start(&t2, mgr);
  em_run(mgr);
  timer_del(&t1);
  timer_del(&t2);
  em_del(mgr);
  hm_del(&env_vars);
  return 0;
}
