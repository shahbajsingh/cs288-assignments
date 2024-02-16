#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// scan array from user input and read into array
void scanArray(int *array, int count) {
    printf("Enter the integers to be sorted:\n");
    for (int i = 0; i < count; i++) {
        scanf("%d", &array[i]);
    }
}

// radix sort using 16 bins
void rad_sort(int *array, int count) {
    int counter[16];
    int offsets[16];

    memset(counter, 0, 16); // set counter to 0
    memset(offsets, 0, 16); // set offsets to 0

    int mask = 0xF;

    int i, j, bin, offset;
    int *buffer = (int *)malloc(sizeof(int) * 100); // buffer to store sorted array

    for (i = 0; i < count; i++) {
        bin = (array[i] & mask) >> 0; // i * 4 does not work
        counter[bin]++;
    }

    for (i = 1; i < 16; i++) {
        // add previous offset to current offset
        offsets[i] = offsets[i-1] + counter[i-1];
    }

    // copy array to buffer
    for (i = 0; i < count; i++) {
        bin = (array[i] & mask) >> 0;
        offset = offsets[bin]++;
        buffer[offset] = array[i];
    }

    memcpy(array, buffer, sizeof(int) * 100); // copy sorted array to array
    free(buffer);
}

int main() {
    printf("Enter number of integers to be sorted: ");
    int count;
    scanf("%d", &count);

    int *array = (int *)malloc(sizeof(int) * 100);

    scanArray(array, count);
    rad_sort(array, count);

    printf("\nSorted integers:\n");
    for (int i = 0; i < count; i++) {
        printf("%d\n", array[i]);
    }

    free(array);
    return 0;
}