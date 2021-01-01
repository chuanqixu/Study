#include "transpose.h"

/* The naive transpose function as a reference. */
void transpose_naive(int n, int blocksize, int *dst, int *src) {
    for (int x = 0; x < n; x++) {
        for (int y = 0; y < n; y++) {
            dst[y + x * n] = src[x + y * n];
        }
    }
}

/* Implement cache blocking below. You should NOT assume that n is a
 * multiple of the block size. */
void transpose_blocking(int n, int blocksize, int *dst, int *src) {
    // YOUR CODE HERE
    for(int i = 0; i <= n/blocksize; i++)
        for(int j = 0; j <= n/blocksize;j++)
            for (int x = 0; x < blocksize; x++)
                for (int y = 0; y < blocksize; y++){
                    int idx_d = i*blocksize*n + j*blocksize + y + x * n;
                    int idx_s = i*blocksize + j*blocksize*n + x + y * n;
                    if(idx_d < n*n && idx_s < n*n){
                        dst[idx_d] = src[idx_s];
                    } else {
                        break;
                    }
                }
}
