#include "kernel/param.h"
#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int g(int x) {
  return x+3;
}

int f(int x) {
  return g(x);
}

void main(void) {
  printf("%d %d\n", f(8)+1, 13);

  // registers are stored in a0, a1, and a2

  // call to f and g is inlined, i.e., there is no call and values are added directly

  // printf is located at the place in the comment in call.asm
  
  // jalr will store the next instruction address to ra

  // unsigned int i = 0x00646c72;
	// printf("H%x Wo%s", 57616, &i); // HE110 World
  // 57616 = 0xE110, 72 -> r, 6c -> l, 62 -> d, 00 -> \n

  // printf("x=%d y=%d", 3); // x=3 y=8229
  // the second %d will use the values in a2, which is the garbage value

  exit(0);
}
