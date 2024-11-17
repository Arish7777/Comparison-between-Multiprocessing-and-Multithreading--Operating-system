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

void merge_sort(int arr[], int l, int r)
{
    if (l < r) {
        int m = l + (r - l) / 2;
        int status;

        pid_t left_pid, right_pid;

        left_pid = fork();
        if (left_pid == 0) {
            merge_sort(arr, l, m);
            _exit(0);
        } else if (left_pid == -1) {
            perror("fork");
            exit(1);
        }

        right_pid = fork();
        if (right_pid == 0) {
            merge_sort(arr, m + 1, r);
            _exit(0);
        } else if (right_pid == -1) {
            perror("fork");
            exit(1);
        }

        waitpid(left_pid, &status, 0);
        waitpid(right_pid, &status, 0);

        merge(arr, l, m, r);
    }
}

int main()
{
	clock_t start, end;
    double cpu_time_used;
        
    int arr[10] = {1,5,2,3,8,5,3,6,6,9};
    int n = sizeof(arr) / sizeof(arr[0]);
start = clock();
    merge_sort(arr, 0, n - 1);
    end = clock();
    measure_memory_usage();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("\nExecution time: %f seconds for mergesort process\n", cpu_time_used);
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

