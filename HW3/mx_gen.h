#ifndef MX_GEN_H
#define MX_GEN_H

#define MAX 100

void generate_rand_mx(int size, int mx[MAX][MAX], int min, int max);
void generate_ident_mx(int size, int mx[MAX][MAX]);
void print_mx(int size, int mx[MAX][MAX]);
void generate_2_mx(int size, int mx1[MAX][MAX], int mx2[MAX][MAX], int min, int max);
void generate_mx_imx(int size, int mx1[MAX][MAX], int mx2[MAX][MAX]);
void mult_mx(int size, int mx1[MAX][MAX], int mx2[MAX][MAX], int rmx[MAX][MAX]);
void write_mx(char *filename, int size, int mx[MAX][MAX]);
void write_mx_bin(char *filename, int size, int mx[MAX][MAX]);
void write_mx_product_bin(char *filename, int size, int mx1[MAX][MAX], int mx2[MAX][MAX]);
void generate_mx_file(char *filename, int size, int min, int max);
void generate_imx_file(char *filename, int size);

#endif /* MX_GEN_H */