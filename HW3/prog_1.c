#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "mx_gen.h"
#define MAX 100

void read_matrix(char *filename, int size, int matrix[MAX][MAX])
{
    // first char is size
    // next chars are matrix
    FILE *file;
    int i, j, dimension;
    file = fopen(filename, "rb");
    fread(&dimension, sizeof(int), 1, file); // Read the dimension of the matrix

    if (size != dimension)
    {
        printf("Error: Incompatible matrix dimensions for multiplication.\n");
        fclose(file);
        exit(1);
    }

    if (size > MAX)
    {
        printf("Error: Matrix size exceeds the maximum limit.\n");
        fclose(file);
        exit(1);
    }

    for (i = 0; i < size; i++)
    {
        for (j = 0; j < size; j++)
        {
            fread(&matrix[i][j], sizeof(int), 1, file); // Read each element of the matrix
        }
    }

    fclose(file);
}

void mult_mx(int size, int mx1[MAX][MAX], int mx2[MAX][MAX], int rmx[MAX][MAX])
{
    int i, j, k;

    for (i = 0; i < size; i++)
    {
        for (j = 0; j < size; j++)
        {
            rmx[i][j] = 0;
            for (k = 0; k < size; k++)
            {
                rmx[i][j] += mx1[i][k] * mx2[k][j];
            }
        }
    }
}

void write_mx_product_bin(char *filename, int size, int mx1[MAX][MAX], int mx2[MAX][MAX])
{
    int rmx[MAX][MAX];
    mult_mx(size, mx1, mx2, rmx);
    write_mx_bin(filename, size, rmx);
}

int main()
{
    int size;
    int mx1[MAX][MAX], mx2[MAX][MAX], rmx[MAX][MAX];
    printf("Enter size of matrix: ");
    scanf("%d", &size);
    generate_mx_file("matrix1.bin", size, 10, 99);
    generate_imx_file("matrix2.bin", size);
    read_matrix("matrix1.bin", size, mx1);
    read_matrix("matrix2.bin", size, mx2);
    write_mx_product_bin("result.bin", size, mx1, mx2);
}