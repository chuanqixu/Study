// Physical memory allocator, for user processes,
// kernel stacks, page-table pages,
// and pipe buffers. Allocates whole 4096-byte pages.

#include "types.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "riscv.h"
#include "defs.h"

void freerange(void *pa_start, void *pa_end);

extern char end[]; // first address after kernel.
                   // defined by kernel.ld.

struct run {
  struct run *next;
};

struct {
  struct spinlock locks[NCPU];
  struct run *freelists[NCPU];
} kmem;

void
kinit()
{
  for (int i = 0; i < NCPU; ++i)
    initlock(&kmem.locks[i], "kmem");
  freerange(end, (void*)PHYSTOP);
}

void
freerange(void *pa_start, void *pa_end)
{
  char *p;
  p = (char*)PGROUNDUP((uint64)pa_start);
  for(; p + PGSIZE <= (char*)pa_end; p += PGSIZE)
    kfree(p);
}

// Free the page of physical memory pointed at by pa,
// which normally should have been returned by a
// call to kalloc().  (The exception is when
// initializing the allocator; see kinit above.)
void
kfree(void *pa)
{
  struct run *r;

  if(((uint64)pa % PGSIZE) != 0 || (char*)pa < end || (uint64)pa >= PHYSTOP)
    panic("kfree");

  // Fill with junk to catch dangling refs.
  memset(pa, 1, PGSIZE);

  r = (struct run*)pa;

  push_off();
  int i_cpu = cpuid();
  pop_off();

  acquire(&kmem.locks[i_cpu]);
  r->next = kmem.freelists[i_cpu];
  kmem.freelists[i_cpu] = r;
  release(&kmem.locks[i_cpu]);
}

// Allocate one 4096-byte page of physical memory.
// Returns a pointer that the kernel can use.
// Returns 0 if the memory cannot be allocated.
void *
kalloc(void)
{
  struct run *r;

  push_off();
  int i_cpu = cpuid();
  pop_off();

  acquire(&kmem.locks[i_cpu]);
  r = kmem.freelists[i_cpu];
  if(r) {
    kmem.freelists[i_cpu] = r->next;
    release(&kmem.locks[i_cpu]);
  } else {
    // here I only implement to steal one page from the other process
    // it can also be implemented to steal many pages, maybe it is faster

    // acquire locks with ascending indices to keep the global lock order and protect from deadlock
    // so first release the lock
    release(&kmem.locks[i_cpu]);
    for (int i = 0; i < NCPU; ++i) {
      if (i == i_cpu) continue;

      int i_small = i < i_cpu ? i : i_cpu, i_large = i > i_cpu ? i : i_cpu;
      acquire(&kmem.locks[i_small]);
      acquire(&kmem.locks[i_large]);
      if (kmem.freelists[i] == 0) {
        release(&kmem.locks[i_large]);
        release(&kmem.locks[i_small]);
        continue;
      }

      // because between release and acquire above, the thread does not hold any lock
      // now the freelist may not be empty, but it does not matter because we only steal one page
      r = kmem.freelists[i];
      kmem.freelists[i] = kmem.freelists[i]->next;
      r->next = 0;

      release(&kmem.locks[i_large]);
      release(&kmem.locks[i_small]);
      break;
    }
  }

  if(r)
    memset((char*)r, 5, PGSIZE); // fill with junk
  return (void*)r;
}
