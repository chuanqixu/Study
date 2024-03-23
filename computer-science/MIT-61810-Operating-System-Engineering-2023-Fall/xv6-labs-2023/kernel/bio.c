// Buffer cache.
//
// The buffer cache is a linked list of buf structures holding
// cached copies of disk block contents.  Caching disk blocks
// in memory reduces the number of disk reads and also provides
// a synchronization point for disk blocks used by multiple processes.
//
// Interface:
// * To get a buffer for a particular disk block, call bread.
// * After changing buffer data, call bwrite to write it to disk.
// * When done with the buffer, call brelse.
// * Do not use the buffer after calling brelse.
// * Only one process at a time can use a buffer,
//     so do not keep them longer than necessary.


#include "types.h"
#include "param.h"
#include "spinlock.h"
#include "sleeplock.h"
#include "riscv.h"
#include "defs.h"
#include "fs.h"
#include "buf.h"



#define NBUCKET 13
struct {
  // this method simply multiplies the bcache and do the same thing for each bucket
  // the disadvantage is that it needs NBUCKET * NBUF cache buffers, which may be too large

  struct spinlock locks[NBUCKET];
  struct buf heads[NBUCKET][NBUF];

} bcache;

void
binit(void)
{
  for (int i = 0; i < NBUCKET; ++i)
    initlock(&bcache.locks[i], "bcache");
}

// Look through buffer cache for block on device dev.
// If not found, allocate a buffer.
// In either case, return locked buffer.
static struct buf*
bget(uint dev, uint blockno)
{
  struct buf *b, *b_empty = 0;

  int i_bucket = blockno % NBUCKET;

  acquire(&bcache.locks[i_bucket]);

  // Is the block already cached?
  for(int i = 0; i < NBUF; ++i){
    b = &bcache.heads[i_bucket][i];
    if(b->dev == dev && b->blockno == blockno){
      b->refcnt++;
      release(&bcache.locks[i_bucket]);
      acquiresleep(&b->lock);
      return b;
    } else if (b->refcnt == 0) {
      b_empty = b;
    }
  }

  // Not cached.
  if(b_empty) {
    b = b_empty;
    b->dev = dev;
    b->blockno = blockno;
    b->valid = 0;
    b->refcnt = 1;
    release(&bcache.locks[i_bucket]);
    acquiresleep(&b->lock);
    return b;
  }
  panic("bget: no buffers");
}

// Return a locked buf with the contents of the indicated block.
struct buf*
bread(uint dev, uint blockno)
{
  struct buf *b;

  b = bget(dev, blockno);
  if(!b->valid) {
    virtio_disk_rw(b, 0);
    b->valid = 1;
  }
  return b;
}

// Write b's contents to disk.  Must be locked.
void
bwrite(struct buf *b)
{
  if(!holdingsleep(&b->lock))
    panic("bwrite");
  virtio_disk_rw(b, 1);
}

// Release a locked buffer.
// Move to the head of the most-recently-used list.
void
brelse(struct buf *b)
{
  if(!holdingsleep(&b->lock))
    panic("brelse");

  releasesleep(&b->lock);

  int i_bucket = b->blockno % NBUCKET;
  acquire(&bcache.locks[i_bucket]);
  b->refcnt--;
  release(&bcache.locks[i_bucket]);
}

void
bpin(struct buf *b) {
  int i_bucket = b->blockno % NBUCKET;
  acquire(&bcache.locks[i_bucket]);
  b->refcnt++;
  release(&bcache.locks[i_bucket]);
}

void
bunpin(struct buf *b) {
  int i_bucket = b->blockno % NBUCKET;
  acquire(&bcache.locks[i_bucket]);
  b->refcnt--;
  release(&bcache.locks[i_bucket]);
}


