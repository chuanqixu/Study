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
#define FIXED_NBUF ((NBUF / NBUCKET) + 2)
struct {
  // keep a list of free buffers
  // when bget cannot find a buffer with refcnt = 0 in the bucket, it will steal
  // one buffer from the free list; when brelse decrease a buffer's refcnt to 0,
  // it add it to the free list

  // this method works out, however, the grading system will timeout
  // this is because brelse add every bucket into the free list when refcnt = 0
  // so each bucket will never keep a buffer with refcnt = 0
  // every time it sees a cache miss, it needs to lock the free list and get one buffer

  // this can be relieved by adding a set of fixed buffers to each bucket
  // these fixed buffers will still be in the bucket and not moved to the free list
  // even when refcnt = 0

  // keep one lock for each bucket
  struct spinlock locks[NBUCKET];

  // fixed_buf is the buffer that are fixed for each bucket and will not be added
  // into the list of free_head even when refcnt = 0
  // this will add at most NBUF + 2 * NBUCKET buffers to the cache
  struct buf fixed_buf[NBUCKET][FIXED_NBUF];

  // this is the same as before
  // buffers in buf are public and can be used by all buckets
  struct buf buf[NBUF];
  struct buf heads[NBUCKET];

  // keep the list of free buffer
  struct spinlock free_lock;
  struct buf free_head;
} bcache;

void
binit(void)
{
  for (int i = 0; i < NBUCKET; ++i)
    initlock(&bcache.locks[i], "bcache");
  initlock(&bcache.free_lock, "bcache");

  // add fixed_buf into each head
  for (int i = 0; i < NBUCKET; ++i) {
    bcache.heads[i].next = &bcache.fixed_buf[i][0];
    bcache.fixed_buf[i][0].prev = &bcache.heads[i];
    bcache.fixed_buf[i][0].next = &bcache.fixed_buf[i][1];
    for (int j = 1; j < FIXED_NBUF - 1; ++j) {
      bcache.fixed_buf[i][j].prev = &bcache.fixed_buf[i][j - 1];
      bcache.fixed_buf[i][j].next = &bcache.fixed_buf[i][j + 1];
    }
    bcache.fixed_buf[i][FIXED_NBUF - 1].prev = &bcache.fixed_buf[i][FIXED_NBUF - 2];
  }

  // add all bcache.buf into bcache.free_head
  bcache.free_head.next = &bcache.buf[0];
  bcache.buf[0].prev = &bcache.free_head;
  bcache.buf[0].next = &bcache.buf[1];
  for (int i = 1; i < NBUF - 1; ++i) {
    bcache.buf[i].next = &bcache.buf[i + 1];
    bcache.buf[i].prev = &bcache.buf[i - 1];
  }
  bcache.buf[NBUF - 1].prev = &bcache.buf[NBUF - 2];
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
  for(b = bcache.heads[i_bucket].next; b; b = b->next){
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
  } else {
    // get one buffer from the free list
    acquire(&bcache.free_lock);
    b = bcache.free_head.next;
    if (b == 0)
      panic("bget: no buffers");

    bcache.free_head.next = b->next;
    if (b->next) b->next->prev = &bcache.free_head;

    b->next = bcache.heads[i_bucket].next;
    b->prev = &bcache.heads[i_bucket];

    bcache.heads[i_bucket].next = b;
    if (b->next) b->next->prev = b;

    release(&bcache.free_lock);
  }
  b->dev = dev;
  b->blockno = blockno;
  b->valid = 0;
  b->refcnt = 1;
  release(&bcache.locks[i_bucket]);
  acquiresleep(&b->lock);
  return b;
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
  // move the buffer to the free list when the it is not the fixed buffer
  if (b->refcnt == 0 && b >= bcache.buf) {
    acquire(&bcache.free_lock);

    b->prev->next = b->next;
    if (b->next) b->next->prev = b->prev;

    b->next = bcache.free_head.next;
    b->prev = &bcache.free_head;

    bcache.free_head.next = b;
    if (b->next) b->next->prev = b;

    release(&bcache.free_lock);
  }

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


