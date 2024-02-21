#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  int p[2];
  if(pipe(p) < 0)
    fprintf(2, "Fail to create pipe!\n");

  if (fork() == 0) {
    char buff[1];
    if (read(p[0], buff, 1) != 1) {
      fprintf(2, "Child fails to read!\n");
      exit(1);
    }

    printf("%d: received ping\n", getpid());

    if (write(p[1], buff, 1) != 1) {
      fprintf(2, "Child fails to write!\n");
      exit(1);
    }
  } else {
    char buff[1];
    *buff = 'a';

    if (write(p[1], buff, 1) != 1) {
      fprintf(2, "Parent fails to write!\n");
      exit(1);
    }

    wait(0);
    printf("%d: received pong\n", getpid());

    if (read(p[0], buff, 1) != 1) {
      fprintf(2, "Parent fails to read!\n");
      exit(1);
    }
  }

  exit(0);
}
