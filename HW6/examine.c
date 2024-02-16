#include <stdio.h>
#include <ctype.h>
#include <string.h>

// print memory in hex format using %02hhx
void print_mem_hex(const unsigned char *ptr, size_t size)
{
    for (size_t i = 0; i < size; i++)
    {
        printf("%02hhx ", *(ptr + i));
    }
}

// print memory in hex format using %02hhx and %c
void print_mem_char(const unsigned char *ptr, size_t size)
{
    for (size_t i = 0; i < size; i++)
    {
        // ternary operation to check if char is printable
        printf("%02hhx(%c) ", *(ptr + i), isprint(*(ptr + i)) ? *(ptr + i) : '.');
        if ((i + 1) % 8 == 0)
        {
            printf(" ");
        }
    }
}

int main(int argc, char *argv[])
{
    printf("argv    | ");
    print_mem_hex((unsigned char *)&argv, sizeof(char **));
    printf(" | %p\n\n", (void *)&argv);

    for (int i = 0; i < argc; i++)
    {
        printf("argv[%d] | ", i);
        print_mem_hex((unsigned char *)(argv + i), sizeof(char *));
        printf(" | %p\n", (void *)(argv + i));
    }

    printf("\n");

    // find start and end address of command line parameters in argv
    unsigned char *argv_st = (unsigned char *)argv[0];
    unsigned char *argv_end = (unsigned char *)argv[argc - 1] + strlen(argv[argc - 1]);

    // align starting address to 8 bytes
    unsigned long long int addr_st = (unsigned long long)argv_st;
    unsigned long long int addr_end = (unsigned long long)(argv_end + 8); // move to end of last string
    
    unsigned char *align_st = (unsigned char *)(addr_st & ~7);
    unsigned char *align_end = (unsigned char *)(addr_end & ~7);

    printf("        | ");
    size_t bytes = 0;
    for (unsigned char *ptr = align_st; ptr < align_end; ptr++)
    {
        printf("%02hhx(%c) ", *ptr, isprint(*ptr) ? *ptr : '.');
        if ((bytes + 1) % 8 == 0)
        {
            printf(" ");
        }
        if ((bytes + 1) % 16 == 0)
        {
            printf("\n        | ");
        }
        bytes++;
    }
    printf(" | %p\n", (void *)align_st);

    return 0;
}
