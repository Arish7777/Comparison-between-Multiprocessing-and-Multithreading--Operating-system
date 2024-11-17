#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <sys/resource.h>
#include <sys/wait.h>
#include <unistd.h>
#define MAX_THREADS 4 // maximum number of threads to use

void measure_memory_usage();
void measure_memory_usage() {
    struct rusage usage;

    getrusage(RUSAGE_SELF, &usage);

    printf("Memory usage: %ld KB\n", usage.ru_maxrss);
}
// the array to search through
int arr[] = {1, 3, 5, 7, 9, 11, 13, 15, 17, 19};

// the number of elements in the array
int n = 10;

// the value to search for
int search_val = 11;

// the index where the value was found, initialized to -1 (not found)
int found_index = -1;

// the mutex lock for mutual exclusion
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void* binary_search(void* args) {
    int thread_num = *(int*) args;
    int start = thread_num * (n / MAX_THREADS);
    int end = (thread_num + 1) * (n / MAX_THREADS) - 1;
    int mid;

    while (start <= end) {
        mid = (start + end) / 2;
        if (arr[mid] == search_val) {
            // found the value, set the found_index and exit
            pthread_mutex_lock(&mutex);
            if (found_index == -1) {
                found_index = mid;
            }
            pthread_mutex_unlock(&mutex);
            return NULL;
        }
        else if (arr[mid] < search_val) {
            start = mid + 1;
        }
        else {
            end = mid - 1;
        }
    }

    return NULL;
}

int main() {
    int arr[] = {1, 3, 5, 7, 9, 11, 13, 15, 17, 19};

clock_t start, end;
    double cpu_time_used;
    pthread_t threads[MAX_THREADS];
    int thread_nums[MAX_THREADS];

    // create threads
    start = clock();
    for (int i = 0; i < MAX_THREADS; i++) {
        thread_nums[i] = i;
        
        pthread_create(&threads[i], NULL, binary_search, &thread_nums[i]);
    }
    measure_memory_usage();

    // join threads
    for (int i = 0; i < MAX_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    // check if the value was found
    if (found_index == -1) {
        printf("Value not found.\n");
    }
    else {
        printf("Value found at index %d.\n", found_index);
    }
        end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("Execution time: %f seconds for binary search threads\n", cpu_time_used);
       struct rusage usage;

    getrusage(RUSAGE_SELF, &usage);
            // open file for writing
    FILE *file = fopen("output.txt", "w");
    if (file == NULL) {
        printf("Error opening file!");
        exit(1);
    }
    

    // write cpu_time_used variable to file
    fprintf(file, "%f ", cpu_time_used);
    fprintf(file, "%ld ", usage.ru_maxrss);
     

    // close file
    fclose(file);

    return 0;
}

