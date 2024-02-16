#include <stdio.h>

#define MAX_SIZE 10000

int main()
{
    int n1, m1, n2, m2, m3 = 0;
    int values1[MAX_SIZE], values2[MAX_SIZE], result_values[2 * MAX_SIZE];
    int column_indices1[MAX_SIZE], column_indices2[MAX_SIZE], result_column_indices[2 * MAX_SIZE];
    int row_start1[MAX_SIZE + 1], row_start2[MAX_SIZE + 1], result_row_start[MAX_SIZE + 1];

    // Open sparse_matrix1.txt and sparse_matrix2.txt for reading
    FILE *mx1 = fopen("sparse_matrix1.txt", "r");
    FILE *mx2 = fopen("sparse_matrix2.txt", "r");

    // Read the number of rows and non-zero values of the matrices
    fscanf(mx1, "%d", &n1);
    fscanf(mx2, "%d", &n2);
    fscanf(mx1, "%d", &m1);
    fscanf(mx2, "%d", &m2);

    /*
    n1: number of rows of sparse_matrix1
    m1: number of non-zero values of sparse_matrix1
    n2: number of rows of sparse_matrix2
    m2: number of non-zero values of sparse_matrix2
    m3: number of non-zero values of the result matrix
    */

    // Read the elements of the matrices (values, column indices, row starts)
    for (int i = 0; i < m1; i++)
    {
        fscanf(mx1, "%d", &values1[i]);
    }
    for (int i = 0; i < m2; i++)
    {
        fscanf(mx2, "%d", &values2[i]);
    }
    for (int i = 0; i < m1; i++)
    {
        fscanf(mx1, "%d", &column_indices1[i]);
    }
    for (int i = 0; i < m2; i++)
    {
        fscanf(mx2, "%d", &column_indices2[i]);
    }
    for (int i = 0; i <= n1; i++)
    {
        fscanf(mx1, "%d", &row_start1[i]);
    }
    for (int i = 0; i <= n2; i++)
    {
        fscanf(mx2, "%d", &row_start2[i]);
    }

    // Close the input files
    fclose(mx1);
    fclose(mx2);

    // Perform sparse matrix addition

    // reconstruct matrix 1 and matrix 2 and store their values in a 2D array

    int idx1 = 0;
    int idx2 = 0;

    int temp_mx1[n1][n1]; // matrix 1 and matrix 2 are used to store
    int temp_mx2[n2][n2]; // the sparse matrices in 2D arrays
    
    for (int i = 0; i < n1; i++) {
        for (int j = 0; j < n1; j++) {
            if (column_indices1[idx1] == j) {
                temp_mx1[i][j] = values1[idx1];
                idx1++;
            }
            else {
                temp_mx1[i][j] = 0;
            }
        }
    }

    for (int i = 0; i < n2; i++) {
        for (int j = 0; j < n2; j++) {
            if (column_indices2[idx2] == j) {
                temp_mx2[i][j] = values2[idx2];
                idx2++;
            }
            else {
                temp_mx2[i][j] = 0;
            }
        }
    }

    // add matrix 1 and matrix 2 and store the result in a 2D array

    int temp_result_mx[n1][n1];  // result is used to store the result of the addition
                                 // as a 2D array

    for (int i = 0; i < n1; i++) {
        for (int j = 0; j < n1; j++) {
            temp_result_mx[i][j] = temp_mx1[i][j] + temp_mx2[i][j];
        }
    }

    // Get CSR representation of the result matrix
    int count = 0;
    result_row_start[0] = 0;
    for (int i = 0; i < n1; i++)
    {
        for (int j = 0; j < n1; j++)
        {
            if (temp_result_mx[i][j] != 0)
            {
                result_values[count] = temp_result_mx[i][j];
                result_column_indices[count] = j;
                count++;
            }
        }
        result_row_start[i + 1] = count;
    }

    m3 = count; // number of non-zero values of the result matrix

    // Open result.txt for writing
    FILE *outfile = fopen("result.txt", "w");

    // Write the number of rows and non-zero values of the result matrix
    fprintf(outfile, "%d\n%d\n", n1, m3);

    // Write the elements of the result matrix (values, column indices, row starts)
    for (int i = 0; i < m3; i++)
    {
        fprintf(outfile, "%d\n", result_values[i]);
    }
    for (int i = 0; i < m3; i++)
    {
        fprintf(outfile, "%d\n", result_column_indices[i]);
    }
    for (int i = 0; i <= n1; i++)
    {
        fprintf(outfile, "%d\n", result_row_start[i]);
    }

    // Close the output file
    fclose(outfile);

    return 0;
}
