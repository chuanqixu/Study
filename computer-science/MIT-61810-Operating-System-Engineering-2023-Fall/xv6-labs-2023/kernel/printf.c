//
// formatted console output -- printf, panic.
//

#include <stdarg.h>

#include "types.h"
#include "param.h"
#include "spinlock.h"
#include "sleeplock.h"
#include "fs.h"
#include "file.h"
#include "memlayout.h"
#include "riscv.h"
#include "defs.h"
#include "proc.h"

volatile int panicked = 0;

// lock to avoid interleaving concurrent printf's.
static struct {
  struct spinlock lock;
  int locking;
} pr;

static char digits[] = "0123456789abcdef";

static void
printint(int xx, int base, int sign)
{
  char buf[16];
  int i;
  uint x;

  if(sign && (sign = xx < 0))
    x = -xx;
  else
    x = xx;

  i = 0;
  do {
    buf[i++] = digits[x % base];
  } while((x /= base) != 0);

  if(sign)
    buf[i++] = '-';

  while(--i >= 0)
    consputc(buf[i]);
}

static void
printptr(uint64 x)
{
  int i;
  consputc('0');
  consputc('x');
  for (i = 0; i < (sizeof(uint64) * 2); i++, x <<= 4)
    consputc(digits[x >> (sizeof(uint64) * 8 - 4)]);
}

// Print to the console. only understands %d, %x, %p, %s.
void
printf(char *fmt, ...)
{
  va_list ap;
  int i, c, locking;
  char *s;

  locking = pr.locking;
  if(locking)
    acquire(&pr.lock);

  if (fmt == 0)
    panic("null fmt");

  va_start(ap, fmt);
  for(i = 0; (c = fmt[i] & 0xff) != 0; i++){
    if(c != '%'){
      consputc(c);
      continue;
    }
    c = fmt[++i] & 0xff;
    if(c == 0)
      break;
    switch(c){
    case 'd':
      printint(va_arg(ap, int), 10, 1);
      break;
    case 'x':
      printint(va_arg(ap, int), 16, 1);
      break;
    case 'p':
      printptr(va_arg(ap, uint64));
      break;
    case 's':
      if((s = va_arg(ap, char*)) == 0)
        s = "(null)";
      for(; *s; s++)
        consputc(*s);
      break;
    case '%':
      consputc('%');
      break;
    default:
      // Print unknown % sequence to draw attention.
      consputc('%');
      consputc(c);
      break;
    }
  }
  va_end(ap);

  if(locking)
    release(&pr.lock);
}

void
panic(char *s)
{
  pr.locking = 0;
  printf("panic: ");
  printf(s);
  printf("\n");

  printf("\nbacktrace:\n");
  backtrace();

  panicked = 1; // freeze uart output from other CPUs  
  for(;;)
    ;
}

void
printfinit(void)
{
  initlock(&pr.lock, "pr");
  pr.locking = 1;
}

void backtrace() {
  // backtrace note down the calling stacks inside the kernel and cannot do with the user calling stacks

  // user call system calls through system call handles in the user space (user/user.h) -> trampoline (kernel/trampoline.S)
  //                -> usertrap (kernel/trap.c) -> syscall (kernel/syscall.c) -> specific system call

  // s0 stores the address of the beginning of each function call, and its return address may be stored in (s0 - 8)
  // (if no function call, it may be optimized), and the previous s0 is stored in (s0 - 16)

  // the first calling happens in the user space from the running function to the system call handle, which changes ra to the 
  // following instruction of that function
  // then it calls 'ecall' and jumps to trampoline, which does not change the value in ra; however, sp is changed to kernel sp,
  // which points to the start of the kernel stack
  // then it jumps to usertrap (it is jr not jlr, so it does not save ra)

  // so in the last stack frame, its saved ra is the return address of the user virtual address (in this lab, it is 0x12),
  // and its saved s0 is the kernel stack address of that process (in this lab, it is 0x0000003fffffa000)
  // because the stack is growing downward, and it happens to be map into another page, so it can be used as one way to find the last stack frame

  struct proc *p = myproc();
  uint64 s0 = r_fp();
  
  while (s0 != p->trapframe->kernel_sp) {
    uint64 ra = *((uint64 *) (s0 - 8));
    printf("%p\n", ra);
    s0 = *((uint64 *) (s0 - 16));
  }

  // alternative way to determine whether it is the last frame
  // uint64 page = PGROUNDDOWN(s0);
  // while (PGROUNDDOWN(s0) == page) {
  // }
}
