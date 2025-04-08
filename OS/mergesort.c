#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define SIZE 8

int array[SIZE] = {10,40,80,30,20,50,60,70 };
int sorted[SIZE];

// Sorting function for each half
void* sort(void* arg) {
    int start = *(int*)arg;
    int end = start + SIZE / 2;

    for (int i = start; i < end; i++) {
        for (int j = start; j < end - 1; j++) {
            if (array[j] > array[j + 1]) {
                int temp = array[j];
                array[j] = array[j + 1];
                array[j + 1] = temp;
            }
        }
    }

    pthread_exit(0);
}

// Merge both sorted halves
void* merge(void* arg) {
    int i = 0;
    int j = SIZE / 2;
    int k = 0;

    while (i < SIZE / 2 && j < SIZE) {
        if (array[i] < array[j])
            sorted[k++] = array[i++];
        else
            sorted[k++] = array[j++];
    }

    while (i < SIZE / 2)
        sorted[k++] = array[i++];

    while (j < SIZE)
        sorted[k++] = array[j++];

    pthread_exit(0);
}

int main() {
    pthread_t tid1, tid2, tid3;

    int start1 = 0;
    int start2 = SIZE / 2;

    pthread_create(&tid1, NULL, sort, &start1);
    pthread_create(&tid2, NULL, sort, &start2);

    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);

    pthread_create(&tid3, NULL, merge, NULL);
    pthread_join(tid3, NULL);

    printf("Sorted array:\n");
    for (int i = 0; i < SIZE; i++)
        printf("%d ", sorted[i]);
    printf("\n");

    return 0;
}
