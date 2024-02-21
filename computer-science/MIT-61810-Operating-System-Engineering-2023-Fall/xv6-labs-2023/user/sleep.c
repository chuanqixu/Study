#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  if(argc < 2 || argc > 2){
    fprintf(2, "Sleep requires exactly one argument!\n");
    exit(1);
  }

  int time = atoi(argv[1]);
  // if (time < 0){
  //   fprintf(2, "Sleep time must be a positive integer!\n");
  //   exit(1);
  // }
  sleep(time);

  exit(0);
}
