#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <sys/resource.h>
#include <sys/wait.h>
#include <unistd.h>
void p_heapify(int arr[], int n, int i);
void measure_memory_usage();
void p_heapsort(int arr[], int n, int num_procs) ;

void measure_memory_usage() {
    struct rusage usage;

    getrusage(RUSAGE_SELF, &usage);

    printf("Memory usage: %ld KB\n", usage.ru_maxrss);
}
void p_heapify(int arr[], int n, int i) {
    int largest = i; 
    int l = 2*i + 1; 
    int r = 2*i + 2; 

    if (l < n && arr[l] > arr[largest]) {
        largest = l;
    }

    if (r < n && arr[r] > arr[largest]) {
        largest = r;
    }

    if (largest != i) {
        int temp = arr[i];
        arr[i] = arr[largest];
        arr[largest] = temp;
        p_heapify(arr, n, largest);
    }
}
void p_heapsort(int arr[], int n, int num_procs) {
    int pid, status;

    // Create child processes
    for (int i = 0; i < num_procs; i++) {
        pid = fork();
        if (pid == -1) {
            printf("Error: fork failed.\n");
            exit(1);
        }
        else if (pid == 0) {
            int start = (n / num_procs) * i;
            int end = (i == num_procs - 1) ? n : (n / num_procs) * (i + 1);
            for (int j = (end / 2) - 1; j >= start; j--) {
                p_heapify(arr, end, j);
            }
            for (int j = end - 1; j > start; j--) {
                int temp = arr[start];
                arr[start] = arr[j];
                arr[j] = temp;
                p_heapify(arr, start, 0);
            }
            exit(0);
        }
    }

    // Wait for child processes to complete
    for (int i = 0; i < num_procs; i++) {
        wait(&status);
    }

    // Merge the sorted subarrays
    int sorted[n];
    int indexes[num_procs];
    int subarray_size = n / num_procs;
    for (int i = 0; i < num_procs; i++) {
        indexes[i] = i * subarray_size;
    }
    for (int i = 0; i < n; i++) {
        int min_val = arr[indexes[0]];
        int min_index = 0;
        for (int j = 1; j < num_procs; j++) {
            if (indexes[j] < (j + 1) * subarray_size && arr[indexes[j]] < min_val) {
                min_val = arr[indexes[j]];
                min_index = j;
            }
        }
        sorted[i] = min_val;
        indexes[min_index]++;
    }
    for (int i = 0; i < n; i++) {
        arr[i] = sorted[i];
    }
}


int main () {
int n = 10;

	int arr[10] = {1,5,2,3,8,5,3,6,6,9};


clock_t start, end;
    double cpu_time_used;
        start = clock();
    p_heapsort(arr, n, 4);
    measure_memory_usage();

    end = clock();
    
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("\nExecution time: %f seconds for heapsort threads\n", cpu_time_used);
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

    
    }
