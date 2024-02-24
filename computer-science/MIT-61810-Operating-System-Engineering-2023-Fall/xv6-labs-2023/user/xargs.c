#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/param.h"
#include "user/user.h"

void copy(char **dst, char *src) {
  /*
  copy string from src to dst
  */

  *dst = (char *) malloc(sizeof(src));
  if (!dst) {
    fprintf(2, "xargs: malloc failed!\n");
    exit(1);
  }
  memcpy(*dst, src, sizeof(src));
}

int
main(int argc, char *argv[]) {
  char str[1024];
  char *x_argv[MAXARG] = {0};
  char c;
  int i = 0, j = 0;

  // copy argv and remove the first xargs
  while (i + 1 < argc) {
    copy(&x_argv[i], argv[i + 1]);
    ++i;
  }

  while (read(0, &c, sizeof(c)) == sizeof(c)) {
    if (c == ' ') {
      if (j != 0) {
        str[j] = 0;
        copy(&x_argv[i], str);
        ++i;
        j = 0;
      }
    } else if (c == '\n') {
      if (j != 0) {
        str[j] = 0;
        copy(&x_argv[i], str);
      }
      i = argc - 1;
      j = 0;

      int pid = fork();
      if (pid < 0) {
        fprintf(2, "Fail to fork!\n");
        exit(1);
      } else if (pid == 0) {
        exec(x_argv[0], x_argv);
        exit(1);
      } else {
        wait(0);
        while (x_argv[i]) {
          free(x_argv[i]);
          ++i;
        }
        i = argc - 1;
      }
    } else str[j++] = c;
  }

  for (i = 0; i < argc - 1; ++i) {
    free(x_argv[i]);
  }

  exit(0);
}
