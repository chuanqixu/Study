CC=gcc
CFLAGS=--std=c99 -Wall

simd: test_simd.o simd.o
	$(CC) $(CFLAGS) -o simd test_simd.o simd.o

test_simd.o: simd.c simd.h
	$(CC) $(CFLAGS) -c test_simd.c -g

simd.o: simd.c simd.h
	$(CC) $(CFLAGS) -c simd.c -g

clean:
	rm -f simd
	rm -f *.o
