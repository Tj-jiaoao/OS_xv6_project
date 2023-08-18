
struct buf {
  int valid;   
  int disk;   
  uint dev;
  uint blockno;
  struct sleeplock lock;
  uint refcnt;
  uchar data[BSIZE];
  uint timestamps;
};

