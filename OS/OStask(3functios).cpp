#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <limits.h>

// Global variables
double average = 0;
int minimum = INT_MAX;
int maximum = INT_MIN;
int *numbers;
int count;

// Function to calculate average
void* calculate_average(void* arg) {
    int sum = 0;
    for (int i = 0; i < count; i++) {
        sum += numbers[i];
    }
    average = (double)sum / count;
    pthread_exit(NULL);
}

// Function to find minimum
void* find_minimum(void* arg) {
    for (int i = 0; i < count; i++) {
        if (numbers[i] < minimum) {
            minimum = numbers[i];
        }
    }
    pthread_exit(NULL);
}

// Function to find maximum
void* find_maximum(void* arg) {
    for (int i = 0; i < count; i++) {
        if (numbers[i] > maximum) {
            maximum = numbers[i];
        }
    }
    pthread_exit(NULL);
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <list of integers>\n", argv[0]);
        return 1;
    }
    
    count = argc - 1;
    numbers = (int*)malloc(count * sizeof(int));
    if (numbers == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }
    
    for (int i = 0; i < count; i++) {
        numbers[i] = atoi(argv[i + 1]);
    }
    
    pthread_t threads[3];
    
    pthread_create(&threads[0], NULL, calculate_average, NULL);
    pthread_create(&threads[1], NULL, find_minimum, NULL);
    pthread_create(&threads[2], NULL, find_maximum, NULL);
    
    for (int i = 0; i < 3; i++) {
        pthread_join(threads[i], NULL);
    }
    
    printf("The average value is %d\n", (int)average);
    printf("The minimum value is %d\n", minimum);
    printf("The maximum value is %d\n", maximum);
    
    free(numbers);
    return 0;
}