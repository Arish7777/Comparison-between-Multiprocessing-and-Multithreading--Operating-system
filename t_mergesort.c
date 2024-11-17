#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <sys/resource.h>
#include <sys/wait.h>
#include <unistd.h>
#define MAX_SIZE 10


void measure_memory_usage() {
    struct rusage usage;

    getrusage(RUSAGE_SELF, &usage);

    printf("Memory usage: %ld KB\n", usage.ru_maxrss);
}


void merge(int arr[], int l, int m, int r)
{
    int i, j, k;
    int n1 = m - l + 1;
    int n2 = r - m;

    int L[n1], R[n2];

    for (i = 0; i < n1; i++) {
        L[i] = arr[l + i];
    }
    for (j = 0; j < n2; j++) {
        R[j] = arr[m + 1 + j];
    }

    i = 0;
    j = 0;
    k = l;

    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            arr[k] = L[i];
            i++;
        } else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }

    while (i < n1) {
        arr[k] = L[i];
        i++;
        k++;
    }

    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;
    }
}

typedef struct {
    int* arr;
    int l;
    int r;
} MergeArgs;

void* merge_sort(void* args)
{
    MergeArgs* merge_args = (MergeArgs*) args;
    int l = merge_args->l;
    int r = merge_args->r;
    int* arr = merge_args->arr;

    if (l < r) {
        int m = l + (r - l) / 2;

        MergeArgs left_args = {arr, l, m};
        MergeArgs right_args = {arr, m + 1, r};

        pthread_t left_thread, right_thread;
        pthread_create(&left_thread, NULL, merge_sort, &left_args);
        pthread_create(&right_thread, NULL, merge_sort, &right_args);

        pthread_join(left_thread, NULL);
        pthread_join(right_thread, NULL);

        merge(arr, l, m, r);
    }

    pthread_exit(NULL);
}

int main()
{
    int arr[10] = {1,5,2,3,8,5,3,6,6,9};
    int n = sizeof(arr) / sizeof(arr[0]);

    MergeArgs merge_args = {arr, 0, n - 1};
    pthread_t sort_thread;

clock_t start, end;
    double cpu_time_used;
     start = clock();
    pthread_create(&sort_thread, NULL, merge_sort, &merge_args);
    pthread_join(sort_thread, NULL);

       end = clock();
    measure_memory_usage();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("\nExecution time: %f seconds for mergesort threads\n", cpu_time_used);
    printf("\n");  
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

