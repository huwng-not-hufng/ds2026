#include "transfer.h"
#include <stdio.h>
#include <stdlib.h> 
FileChunk *
read_file_1_svc(Params *argp, struct svc_req *rqstp)
{
    static FileChunk result;
    FILE *f;
    char *mem_buffer; 
    result.data.data_val = NULL;
    result.data.data_len = 0;

    printf("Server reading: %s (Offset: %d)\n", argp->filename, argp->offset);

    f = fopen(argp->filename, "rb");
    if (f == NULL) {
        result.bytes_read = 0;
        return &result;
    }

    fseek(f, argp->offset, SEEK_SET);


    mem_buffer = (char *)malloc(4096); 
    
    if (mem_buffer == NULL) {
        fclose(f);
        return NULL; 
    }

    int bytes = fread(mem_buffer, 1, argp->size > 4096 ? 4096 : argp->size, f);
    fclose(f);

    result.data.data_val = mem_buffer;
    result.data.data_len = bytes;
    result.bytes_read = bytes;

    return &result;
}