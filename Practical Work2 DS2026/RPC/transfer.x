/* transfer.x */

struct FileChunk {
    opaque data<>;    
    int bytes_read;     
};


struct Params {
    string filename<>;
    int offset;
    int size;
};

program FILE_TRANSFER_PROG {
    version FILE_TRANSFER_VERS {
        
        FileChunk READ_FILE(Params) = 1;
    } = 1;
} = 0x31230000;