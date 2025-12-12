#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CHUNK_SIZE 4096
#define TAG_DATA 1

int main(int argc, char *argv[]) {
    int rank, size;
    char buffer[CHUNK_SIZE];
    MPI_Status status;

    
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank); 
    MPI_Comm_size(MPI_COMM_WORLD, &size); 

   
    if (size < 2) {
        if (rank == 0) {
            fprintf(stderr, "Error: This program requires at least 2 processes.\n");
            fprintf(stderr, "Usage: mpirun -np 2 ./transfer <filename>\n");
        }
        MPI_Finalize();
        return 1;
    }

    
    if (rank == 0) {
        if (argc < 2) {
            fprintf(stderr, "Usage: mpirun -np 2 ./transfer <filename>\n");
            MPI_Abort(MPI_COMM_WORLD, 1);
        }

        char *filename = argv[1];
        FILE *f = fopen(filename, "rb");
        if (f == NULL) {
            perror("File open error");
            MPI_Abort(MPI_COMM_WORLD, 1);
        }

        printf("[Sender] Reading file: %s\n", filename);
        int bytes_read;
        int total_sent = 0;

        
        while ((bytes_read = fread(buffer, 1, CHUNK_SIZE, f)) > 0) {
            
            MPI_Send(buffer, bytes_read, MPI_BYTE, 1, TAG_DATA, MPI_COMM_WORLD);
            total_sent += bytes_read;
        }

       
        MPI_Send(buffer, 0, MPI_BYTE, 1, TAG_DATA, MPI_COMM_WORLD);
        
        fclose(f);
        printf("[Sender] Finished sending %d bytes.\n", total_sent);
    }

   
    else if (rank == 1) {
        FILE *f_dest = fopen("received.txt", "wb");
        if (f_dest == NULL) {
            perror("File write error");
            MPI_Abort(MPI_COMM_WORLD, 1);
        }

        int total_received = 0;
        int count;

        printf("[Receiver] Waiting for data...\n");

        while (1) {
            
            MPI_Recv(buffer, CHUNK_SIZE, MPI_BYTE, 0, TAG_DATA, MPI_COMM_WORLD, &status);

            
            MPI_Get_count(&status, MPI_BYTE, &count);

            
            if (count == 0) {
                break;
            }

            fwrite(buffer, 1, count, f_dest);
            total_received += count;
        }

        fclose(f_dest);
        printf("[Receiver] File received successfully (%d bytes).\n", total_received);
    }

    MPI_Finalize();
    return 0;
}