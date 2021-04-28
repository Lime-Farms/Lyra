#include <errno.h>
#include <hm.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char **argv, char **envs) {
  int arg = opterr = 0;

  while((arg = getopt(argc, argv, "")) > 0) {
    if(arg == '?') {
      fprintf(stderr, "error: option '-%c' incorrectly used.\n", arg);
      return EINVAL;
    }
  }

  char **env = envs;
  struct hm map = hm_new(0);

  for(; *env != NULL; env += 1) {
    const char *name = strtok(*env, "=");
    size_t len = strlen(name);
    hm_add(&map, name, *env + len + 1);
  }

  return 0;
}
