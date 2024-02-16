#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 100

void radix_sort(int *array, int count){

    unsigned short counter[16];
    int offsets[16];
    int mask = 0xF;
    int i,j, bin, offset;
    int *buffer = (int *)malloc(sizeof(int) * MAX);
    
    for (i=0; i<count; i++){
        bin = (array[i] & mask) >> i * 4;
        counter[bin]++;
    }

    offsets[0] = 0;

    for (i=1; i<16; i++){
        offsets[i] = offsets[i-1] + counter[i-1];
    }

    for (i=0; i<count; i++){
        bin = (array[i] & mask) >> i * 4;
        offset = offsets[bin];
        buffer[offset] = array[i];
        offsets[bin]++;
    }

    // copy buffer to array
    memcpy(array, buffer, sizeof(int) * MAX);
    free(buffer);
    
    
}

int main(){
    printf("Enter the number of integers to be sorted: ");
    int count;
    scanf("%d", &count);

    int *array = (int *)malloc(sizeof(int) * MAX);
    int *buffer = (int *)malloc(sizeof(int) * MAX);

    printf("Enter the integers to be sorted: ");
    for(int i = 0; i < count; i++){
        scanf("%d", &array[i]);
    }

    radix_sort(array, count);

    printf("Sorted integers: ");
    for(int i = 0; i < count; i++){
        printf("%d ", array[i]);
    }
    printf("\n");

    free(array);
    free(buffer);
    return 0;
}
