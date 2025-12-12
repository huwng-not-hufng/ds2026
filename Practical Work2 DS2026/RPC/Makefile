
CC = gcc

CFLAGS = -I/usr/include/tirpc
LIBS = -lnsl -ltirpc

all: client server


gen: transfer.x
	rpcgen -C transfer.x


client: gen client.c
	$(CC) $(CFLAGS) -o client client.c transfer_clnt.c transfer_xdr.c $(LIBS)


server: gen server.c
	$(CC) $(CFLAGS) -o server server.c transfer_svc.c transfer_xdr.c $(LIBS)

clean:
	rm -f client server transfer.h transfer_clnt.c transfer_svc.c transfer_xdr.c