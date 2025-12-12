CC = mpicc
CFLAGS = -Wall

all: transfer

transfer: transfer.c
	$(CC) $(CFLAGS) -o transfer transfer.c

clean:
	rm -f transfer received.txt