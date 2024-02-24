#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
primes(int upper)
{
  /*
  fd 0 will be used for the current process to read from its parent process
  fd 1 will be used for the current process to pass the new potential number to check to its child process

  each process maintain a local variable to store the basis prime number
  continuously read the number from fd 0, and only proceed when the number cannot be divided by the basis prime number of the process
  if the current process has not forked before, it means no children stores the new basis primes number, so fork and the basis prime number of the child process is this number
  otherwise, directly pass the number to the child number to check
  */

  int prime = upper + 1, // basis prime number of the current process
      p[2], // pipes; after fork, 0 will be the fd to read from the parent process, and 1 will be the fd to write to the child process
      pid = 0, // show whether the current process has forked or not. If pid == 0, this is a child; if pid > 0, the current process has forked
      num = 2; // number to read

  while (read(0, &num, sizeof(num)) != 0) {
    if (num % prime) {
      if (pid == 0) { // if it has not forked, it needs to fork
        if (pipe(p) < 0) {
          fprintf(2, "Fail to create pipe!\n");
          exit(1);
        }
        pid = fork();
        if (pid < 0) {
          fprintf(2, "Fail to fork!\n");
          exit(1);
        } else if (pid == 0) {
          close(0);
          dup(p[0]); // fd: child's 0 is the output pipe from the parent process
          close(p[0]);
          close(p[1]);
          if (read(0, &prime, sizeof(prime)) != sizeof(prime)) { // update the child process's prime
            fprintf(2, "Failed to read!\n");
            exit(1);
          }
          fprintf(1, "prime %d\n", prime);
        } else {
          close(1);
          dup(p[1]); // fd: parent's 1 is the input pipe to the child
          close(p[0]);
          close(p[1]);
        }
      }

      if (num % prime) { // child has different basis prime number, so need to check
        if (write(1, &num, sizeof(num)) != sizeof(num)) {
          fprintf(2, "Fail to write!\n");
          exit(1);
        }
      }
    }
  }

  close(1);
  if (pid > 0) {
    wait(0);
  }
  exit(0);
}


int
main(int argc, char *argv[]) {
  int upper = 35, p[2];

  if (pipe(p) < 0) {
    fprintf(2, "Fail to create pipe!\n");
    exit(1);
  }

  int pid = fork();
  if (pid < 0) {
    fprintf(2, "Fail to fork!\n");
    exit(1);
  } else if (pid == 0) {
    close(0);
    dup(p[0]);
    close(p[0]);
    close(p[1]);
    primes(upper);
  } else {
    for (int i = 2; i <= upper; ++i) {
      if (write(p[1], &i, sizeof(i)) != sizeof(i)) {
        fprintf(2, "Fail to write!\n");
        exit(1);
      }
    }
    close(p[0]);
    close(p[1]);
    wait(0);
  }

  exit(0);
}
