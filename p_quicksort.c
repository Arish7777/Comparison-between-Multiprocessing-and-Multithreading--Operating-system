#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <sys/resource.h>
#include <sys/wait.h>
#include <unistd.h>

void p_quicksort(int arr[], int low, int high, int nprocs);
void swap(int *a, int *b);
int p_partition(int arr[], int low, int high);
void measure_memory_usage();

void measure_memory_usage() {
    struct rusage usage;

    getrusage(RUSAGE_SELF, &usage);

    printf("Memory usage: %ld KB\n", usage.ru_maxrss);
}

int p_partition(int arr[], int low, int high) {
    int pivot = arr[high];
    int i = (low - 1);

    for (int j = low; j <= high - 1; j++) {
        if (arr[j] < pivot) {
            i++;
            swap(&arr[i], &arr[j]);
        }
    }
    swap(&arr[i + 1], &arr[high]);
    return (i + 1);
}


void swap(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}
void p_quicksort(int arr[], int low, int high, int nprocs) {
    if (low < high) {
        int pi = p_partition(arr, low, high);
        pid_t pid;
        int status;

        if (nprocs > 1) {
            // create two child processes
            pid = fork();
            if (pid == 0) {
                p_quicksort(arr, low, pi - 1, nprocs/2);
                exit(0);
            }
            else {
                pid = fork();
                if (pid == 0) {
                    p_quicksort(arr, pi + 1, high, nprocs/2);
                    exit(0);
                }
            }
        }
        else {
            p_quicksort(arr, low, pi - 1, 1);
            p_quicksort(arr, pi + 1, high, 1);
        }

        // wait for the child processes to finish
        while (wait(&status) > 0);
    }
}

int main() {
clock_t start, end;
    double cpu_time_used;
    int n = 10;
    int arr[10] = {1, 5, 2, 3, 8, 5, 3, 6, 6, 9};

    start = clock();
    p_quicksort(arr, 0, n-1, 4);
    measure_memory_usage();


    printf("\n");
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("Execution time: %f seconds with multiple process quicksort\n", cpu_time_used);
    
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

