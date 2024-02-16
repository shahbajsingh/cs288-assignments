#include <stdio.h>

#define MAX_COUNT 100
#define NUM_BUCKETS 16
#define NUM_BITS 4

void radixSort(int arr[], int count) {
    int buffer[MAX_COUNT];
    int mask = NUM_BUCKETS - 1;  // Mask to extract the least significant bits

    for (int shift = 0; shift < 32; shift += NUM_BITS) {
        int buckets[NUM_BUCKETS] = {0};

        // Count the number of elements in each bucket
        for (int i = 0; i < count; i++) {
            int bucket = (arr[i] >> shift) & mask;
            buckets[bucket]++;
        }

        // Calculate the starting index for each bucket in the buffer
        int total = 0;
        for (int i = 0; i < NUM_BUCKETS; i++) {
            int temp = buckets[i];
            buckets[i] = total;
            total += temp;
        }

        // Move elements to the buffer according to the current bits
        for (int i = 0; i < count; i++) {
            int bucket = (arr[i] >> shift) & mask;
            buffer[buckets[bucket]] = arr[i];
            buckets[bucket]++;
        }

        // Copy the elements from the buffer back to the original array
        for (int i = 0; i < count; i++) {
            arr[i] = buffer[i];
        }
    }
}

int main() {
    int count;
    int arr[MAX_COUNT];
    int positive[MAX_COUNT];
    int negative[MAX_COUNT];
    int negCount = 0;
    int posCount = 0;

    printf("Enter the count of integers: ");
    scanf("%d", &count);

    if (count > MAX_COUNT) {
        printf("Count exceeds the maximum limit.\n");
        return 0;
    }

    printf("Enter %d integers:\n", count);
    for (int i = 0; i < count; i++) {
        int num;
        scanf("%d", &num);
        if (num < 0)
            negative[negCount++] = num;
        else
            positive[posCount++] = num;
    }

    radixSort(positive, posCount);
    radixSort(negative, negCount);

    printf("Sorted integers:\n");
    for (int i = 0; i <= negCount-1; i++) {
        printf("%d\n", negative[i]);
    }
    
    for (int i = 0; i < posCount; i++) {
        printf("%d\n", positive[i]);
    }

    return 0;
}