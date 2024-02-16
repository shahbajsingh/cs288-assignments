#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "mx_gen.h"
#define MAX 100

void generate_rand_mx(int size, int mx[MAX][MAX], int min, int max)
{
    int i, j, num;
    srand(time(NULL));

    for (i = 0; i < size; i++)
    {
        for (j = 0; j < size; j++)
        {
            num = rand() % (max - min) + min;
            mx[i][j] = num;
        }
    }
}

void generate_ident_mx(int size, int mx[MAX][MAX])
{
    int i, j;

    for (i = 0; i < size; i++)
    {
        for (j = 0; j < size; j++)
        {
            if (i == j)
            {
                mx[i][j] = 1;
            }
            else
            {
                mx[i][j] = 0;
            }
        }
    }
}

void print_mx(int size, int matrix[MAX][MAX])
{
    int i, j;
    for (i = 0; i < size; i++)
    {
        for (j = 0; j < size; j++)
        {
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }
}

void generate_2_mx(int size, int mx1[MAX][MAX], int mx2[MAX][MAX], int min, int max)
{
    generate_rand_mx(size, mx1, min, max);
    generate_rand_mx(size, mx2, min, max);
}

void generate_mx_imx(int size, int mx1[MAX][MAX], int mx2[MAX][MAX])
{
    generate_rand_mx(size, mx1, 0, 100);
    generate_ident_mx(size, mx2);
}

void write_mx(char *filename, int size, int matrix[MAX][MAX])
{
    FILE *file = fopen(filename, "w");
    int i, j;

    fprintf(file, "%d\n", size);
    for (i = 0; i < size; i++)
    {
        for (j = 0; j < size; j++)
        {
            fprintf(file, "%d ", matrix[i][j]);
        }
        fprintf(file, "\n");
    }
    fclose(file);
}

void write_mx_bin(char *filename, int size, int mx[MAX][MAX])
{
    FILE *file = fopen(filename, "wb");
    int i, j;

    fwrite(&size, sizeof(int), 1, file); // write size
    for (i = 0; i < size; i++)
    {
        for (j = 0; j < size; j++)
        {
            fwrite(&mx[i][j], sizeof(int), 1, file);
        }
    }
    fclose(file);
}

void generate_mx_file(char *filename, int size, int min, int max)
{
    int mx[MAX][MAX];
    generate_rand_mx(size, mx, min, max);
    write_mx_bin(filename, size, mx);
}

void generate_imx_file(char *filename, int size)
{
    int mx[MAX][MAX];
    generate_ident_mx(size, mx);
    write_mx_bin(filename, size, mx);
}
