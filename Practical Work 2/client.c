#include "transfer.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    CLIENT *clnt;
    FileChunk  *result;
    Params args; 
    char *host;
    char *local_file;

    if (argc < 4) {
        printf("Usage: %s <host> <remote_file> <local_file>\n", argv[0]);
        exit(1);
    }
    host = argv[1];
    
   
    args.filename = argv[2];
    args.offset = 0;
    args.size = 4096;

    local_file = argv[3];

    clnt = clnt_create(host, FILE_TRANSFER_PROG, FILE_TRANSFER_VERS, "udp");
    if (clnt == NULL) {
        clnt_pcreateerror(host);
        exit(1);
    }

    FILE *f_dest = fopen(local_file, "wb");
    int is_running = 1;

    while (is_running) {
        
        result = read_file_1(&args, clnt);

        if (result == NULL) {
            clnt_perror(clnt, "RPC failed");
            break;
        }

        if (result->bytes_read > 0) {
            fwrite(result->data.data_val, 1, result->data.data_len, f_dest);
            args.offset += result->bytes_read; 
            printf("Received %d bytes...\n", args.offset);
        } else {
            is_running = 0;
        }
    }

    fclose(f_dest);
    clnt_destroy(clnt);
    printf("Done.\n");
    return 0;
}