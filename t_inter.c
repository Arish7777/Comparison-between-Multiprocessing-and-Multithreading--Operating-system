#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <sys/resource.h>
#include <sys/wait.h>
#include <unistd.h>

#define MAX 100000

int arr[MAX];
int n, key=11, found = 0;

void measure_memory_usage() {
    struct rusage usage;

    getrusage(RUSAGE_SELF, &usage);

    printf("Memory usage: %ld KB\n", usage.ru_maxrss);
}

void* search(void* arg) {
    int thread_num = *(int*)arg;
    int low = thread_num * (n / 4);
    int high = low + (n / 4) - 1;

    while (low <= high && key >= arr[low] && key <= arr[high]) {
        if (low == high) {
            if (arr[low] == key) {
                printf("Thread %d found the key at index %d.\n", thread_num, low);
                found = 1;
                break;
            }
            else {
                break;
            }
        }

        int pos = low + (((double)(high - low) / (arr[high] - arr[low])) * (key - arr[low]));
        if (arr[pos] == key) {
            printf("Thread %d found the key at index %d.\n", thread_num, pos);
            found = 1;
            break;
        }
        else if (arr[pos] < key) {
            low = pos + 1;
        }
        else {
            high = pos - 1;
        }
    }
}

int main() {
    int arr[] = {1, 3, 5, 7, 9, 11, 13, 15, 17, 19};

printf("Interpolation search with threads");
clock_t start, end;
    double cpu_time_used;
    pthread_t threads[4];
    int thread_num[4] = {0, 1, 2, 3};


    for (int i = 0; i < 4; i++) {
        pthread_create(&threads[i], NULL, search, (void*)&thread_num[i]);
    }

    for (int i = 0; i < 4; i++) {
        pthread_join(threads[i], NULL);
    }
    measure_memory_usage();

    if (!found) {
        printf("Key not found in the array.\n");
    }
           struct rusage usage;

    getrusage(RUSAGE_SELF, &usage);
            // open file for writing
    FILE *file = fopen("output.txt", "a");
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

