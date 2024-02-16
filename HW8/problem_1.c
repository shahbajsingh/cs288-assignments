#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define MAX 1000

// perform local count sort
void countSortLocal(unsigned int *arr, int size, int *local_count)
{
    for (int i = 0; i < size; i++)
        local_count[arr[i]]++;
}

// perform count sort using MPI
void countSortMPI(char *filename, int numints, int rank, int size)
{
    int i;
    unsigned int *buffer;
    int local_count[MAX] = {0};
    int global_count[MAX] = {0};

    int chunk_size = numints / size; // number of integers in each chunk

    FILE *file = fopen(filename, "rb"); // open the file for reading
    
    if (file == NULL)
    {
        fprintf(stderr, "ERROR: Could not open file %s for reading.\n", filename);
        exit(1);
    }

    // move the file pointer to the start of the chunk
    fseek(file, rank * chunk_size * sizeof(unsigned int), SEEK_SET);

    // memory allocation for the chunk
    buffer = (unsigned int *)malloc(chunk_size * sizeof(unsigned int));
    if (buffer == NULL)
    {
        fprintf(stderr, "ERROR: Could not allocate memory for buffer.\n");
            exit(1);
    }

    // read the chunk from the file
    fread(buffer, sizeof(unsigned int), chunk_size, file);

    fclose(file);

    // perform local count sort
    countSortLocal(buffer, chunk_size, local_count);

    // reduce local counts to global counts
    MPI_Reduce(local_count, global_count, MAX, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    // broadcast the global counts to all processes
    MPI_Bcast(global_count, MAX, MPI_INT, 0, MPI_COMM_WORLD);

    int sum = 0;
    for (i = 0; i < MAX; i++)
    {
        int temp = global_count[i];
        global_count[i] = sum;
        sum += temp;
    }

    // perform local sort
    int *local_sorted = (int *)malloc(chunk_size * sizeof(unsigned int));
    for (i = 0; i < chunk_size; i++)
    {
        local_sorted[global_count[buffer[i]]] = buffer[i];
        global_count[buffer[i]]++;
    }

    // gather sorted values to root process
    unsigned int *sorted_values = NULL;

    if (rank == 0)
        sorted_values = (unsigned int *)malloc(numints * sizeof(unsigned int));
    MPI_Gather(local_sorted, chunk_size, MPI_UNSIGNED, sorted_values, chunk_size, MPI_UNSIGNED, 0, MPI_COMM_WORLD);

    // write sorted values to file
    if (rank == 0)
    {
        FILE *sorted_file = fopen(filename, "wb");
        if (sorted_file == NULL)
        {
            fprintf(stderr, "Error opening the file for writing.\n");
            exit(1);
        }
        fwrite(sorted_values, sizeof(unsigned int), numints, sorted_file);
        fclose(sorted_file);
        free(sorted_values);
    }

    free(buffer);
    free(local_sorted);
}

int main(int argc, char *argv[])
{
    int rank, size;
    int numints;

    if (argc != 2)
    {
        printf("Usage: %s <filename>\n", argv[0]);
        exit(1);
    }

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (rank == 0)
        sscanf(argv[1], "%d", &numints);

    MPI_Bcast(&numints, 1, MPI_INT, 0, MPI_COMM_WORLD);

    countSortMPI(argv[1], numints, rank, size);

    MPI_Finalize();
    return 0;
}
