#include <stdio.h>
#include <stdlib.h>

#define MAX 100
#define BUCKETS 16
#define BITS 4

// radixSort sorts the given array of floating-point values in ascending order
void radixSort(float *arr[], int count) {
    float *buffer = malloc(count * sizeof(float));
    int mask = 0xF; 

    // Sort the array by each group of 4 bits
    for (int shift = 0; shift < 32; shift += BITS) {
        int buckets[BUCKETS] = {0}; // array to store the number of elements in each bucket

        for (int i = 0; i < count; i++) {
            int bucket = (*((unsigned int *)arr[i]) >> shift) & mask;
            buckets[bucket]++; // increment the count of elements in the bucket
        }

        int total = 0;
        for (int i = 0; i < BUCKETS; i++) { // calculate starting index for each bucket in the buffer
            int temp = buckets[i];
            buckets[i] = total;
            total += temp;
        }

        for (int i = 0; i < count; i++) {
            // get the bucket index based on the current bits by casting the float pointer to an unsigned int pointer
            // and shifting the bits to the right by the current shift amount and then masking 
            int bucket = (*((unsigned int *)arr[i]) >> shift) & mask;
            buffer[buckets[bucket]++] = *arr[i];
        }

        for (int i = 0; i < count; i++) {
            *arr[i] = buffer[i]; // copy the elements from the buffer back to the original array
        }
    }

    free(buffer);
}

int main() {
    int count;
    float *arr[MAX];
    float *positive[MAX];
    float *negative[MAX];
    int negCount = 0;
    int posCount = 0;

    printf("Enter number of floats to sort: ");
    scanf("%d", &count);

    if (count > MAX) {
        printf("Count exceeds the maximum limit of 100.\n");
        return 0;
    }

    printf("Enter %d floating-point values:\n", count);
    for (int i = 0; i < count; i++) {
        float *value = malloc(sizeof(float));
        scanf("%f", value);
        if (*value < 0)
            negative[negCount++] = value;
        else
            positive[posCount++] = value;
    }

    radixSort(negative, negCount);
    radixSort(positive, posCount);

    printf("Sorted float values:\n");
    for (int i = negCount-1; i >= 0; i--) {
        printf("%f\n", *negative[i]);
        free(negative[i]);
    }
    
    for (int i = 0; i < posCount; i++) {
        printf("%f\n", *positive[i]);
        free(positive[i]);
    }

    return 0;
}

