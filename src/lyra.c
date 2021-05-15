#define _XOPEN_SOURCE

#include <errno.h>
#include <lyra/hm.h>
#include <stdio.h>
#include <strings.h>
#include <unistd.h>

uint8_t print_env(const char *key, const void *value) {
  printf("%s: %s\n", key, (const char *) value);
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

  hm_foreach(&env_vars, print_env);
  hm_del(&env_vars);
  return 0;
}
