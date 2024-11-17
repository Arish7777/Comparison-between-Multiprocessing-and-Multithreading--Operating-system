#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <sys/resource.h>
#include <sys/wait.h>
#include <unistd.h>

void t_partition(int *arr, int left, int right, int *pivot);
struct arg_struct {
    int *arr;
    int left;
    int right;
};

void *quicksort(void *arg) {
    struct arg_struct *args = (struct arg_struct *)arg;
    int *arr = args->arr;
    int left = args->left;
    int right = args->right;
    if (left >= right) {
        return NULL;
    }
    int pivot;
    t_partition(arr, left, right, &pivot);
    struct arg_struct left_args, right_args;
    left_args.arr = arr;
    left_args.left = left;
    left_args.right = pivot-1;
    right_args.arr = arr;
    right_args.left = pivot+1;
    right_args.right = right;
    pthread_t left_tid, right_tid;
    pthread_create(&left_tid, NULL, quicksort, (void *)&left_args);
    pthread_create(&right_tid, NULL, quicksort, (void *)&right_args);
    pthread_join(left_tid, NULL);
    pthread_join(right_tid, NULL);
    return NULL;
}

void t_partition(int *arr, int left, int right, int *pivot) {
    int i = left + 1;
    int j = right;
    int temp;
    while (i <= j) {
        while (i <= right && arr[i] < arr[left]) {
            i++;
        }
        while (j >= left+1 && arr[j] >= arr[left]) {
            j--;
        }
        if (i <= j) {
            temp = arr[i];
            arr[i] = arr[j];
            arr[j] = temp;
            i++;
            j--;
        }
    }
    temp = arr[left];
    arr[left] = arr[j];
    arr[j] = temp;
    *pivot = j;
}

void measure_memory_usage() {
    struct rusage usage;

    getrusage(RUSAGE_SELF, &usage);

    printf("Memory usage: %ld KB\n", usage.ru_maxrss);
}


int main() {
    int n = 10;
    clock_t start, end;
    double cpu_time_used;
    int arr[10] = {1,5,2,3,8,5,3,6,6,9};
    struct arg_struct args;
    args.arr = arr;
    args.left = 0;
    args.right = n-1;
    pthread_t tid;
    start = clock();
    pthread_create(&tid, NULL, quicksort, (void *)&args);
    measure_memory_usage();
    pthread_join(tid, NULL);

    printf("\n");
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("Execution time: %f seconds for quicksort threads\n", cpu_time_used);
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

