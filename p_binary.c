#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <time.h>
#include <sys/resource.h>
#include <sys/wait.h>

void measure_memory_usage();
void p_binary_search(int arr[], int low, int high, int x, int nprocs, int *shm_result);
int binary_search(int arr[], int low, int high, int x);
void measure_memory_usage() {
    struct rusage usage;

    getrusage(RUSAGE_SELF, &usage);

    printf("Memory usage: %ld KB\n", usage.ru_maxrss);
}
int binary_search(int arr[], int low, int high, int x) {
    while (low <= high) {
        int mid = (low + high) / 2;
        if (arr[mid] == x) {
            return mid;
        }
        else if (arr[mid] < x) {
            low = mid + 1;
        }
        else {
            high = mid - 1;
        }
    }
    return -1;
}

void p_binary_search(int arr[], int low, int high, int x, int nprocs, int *shm_result) {
    if (low <= high) {
        int mid = (low + high) / 2;
        pid_t pid1, pid2;
        int status1, status2;

        pid1 = fork();
        if (pid1 == 0) {
            p_binary_search(arr, low, mid - 1, x, nprocs / 2, shm_result);
            exit(0);
        }
        else {
            pid2 = fork();
            if (pid2 == 0) {
                p_binary_search(arr, mid + 1, high, x, nprocs / 2, shm_result);
                exit(0);
            }
        }

        // wait for child processes to finish
        while (wait(&status1) > 0);
        while (wait(&status2) > 0);

        // check if result is found in child processes
        if (*shm_result != -1) {
            return;
        }

        // check current process for result
        if (arr[mid] == x) {
            *shm_result = mid;
            return;
        }
        else if (arr[mid] < x) {
            p_binary_search(arr, mid + 1, high, x, 1, shm_result);
        }
        else {
            p_binary_search(arr, low, mid - 1, x, 1, shm_result);
        }
    }
}

int main() {
    int n = 10;
     int arr[] = {1, 3, 5, 7, 9, 11, 13, 15, 17, 19};
    int x = 11;
    clock_t start, end;
    double cpu_time_used;
     printf("Original array: ");
    for (int i = 0; i < n; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");

    // create shared memory segment for result
    int shm_id = shmget(IPC_PRIVATE, sizeof(int), IPC_CREAT | 0666);
    int *shm_result = shmat(shm_id, NULL, 0);
    *shm_result = -1;
start = clock();
    // perform binary search with multiple processes
    p_binary_search(arr, 0, n - 1, x, 4, shm_result);
measure_memory_usage();
    // print result
    if (*shm_result == -1) {
        printf("Element not found\n");
    }
    else {
        printf("Element found at index %d\n", *shm_result);
    }
end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("Execution time: %f seconds for binary search process\n", cpu_time_used);
    // detach shared memory segment
    shmdt(shm_result);
    shmctl(shm_id, IPC_RMID, NULL);
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

