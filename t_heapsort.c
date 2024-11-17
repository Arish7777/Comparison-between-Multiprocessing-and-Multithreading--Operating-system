#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <sys/resource.h>
#include <sys/wait.h>
#include <unistd.h>

void heapify(int arr[], int n, int i);
void *heap_sort(void *arg);
void merge(int arr[], int n) ;
void measure_memory_usage();
void swap(int *a, int *b) ;

void swap(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}
void measure_memory_usage() {
    struct rusage usage;

    getrusage(RUSAGE_SELF, &usage);

    printf("Memory usage: %ld KB\n", usage.ru_maxrss);
}
void merge(int arr[], int n) {
    int temp[n];
    int i = 0;
    int j = n / 2;
    int k = 0;
    while (i < n / 2 && j < n) {
        if (arr[i] < arr[j]) {
            temp[k++] = arr[i++];
        } else {
            temp[k++] = arr[j++];
        }
    }
    while (i < n / 2) {
        temp[k++] = arr[i++];
    }
    while (j < n) {
        temp[k++] = arr[j++];
    }
    for (i = 0; i < n; i++) {
        arr[i] = temp[i];
    }
}
void heapify(int arr[], int n, int i) {
    int largest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;
    if (left < n && arr[left] > arr[largest]) {
        largest = left;
    }
    if (right < n && arr[right] > arr[largest]) {
        largest = right;
    }
    if (largest != i) {
        swap(&arr[i], &arr[largest]);
        heapify(arr, n, largest);
    }
}

void *heap_sort(void *arg) {
    int *arr = (int *)arg;
    int n = 10 / 2;
    for (int i = n / 2 - 1; i >= 0; i--) {
        heapify(arr, n, i);
    }
    for (int i = n - 1; i >= 0; i--) {
        swap(&arr[0], &arr[i]);
        heapify(arr, i, 0);
    }
    pthread_exit(NULL);
}

int main() {
 int n = 10;

	int arr[10] = {1,5,2,3,8,5,3,6,6,9};
 printf("Original array: ");
    for (int i = 0; i < n; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");

clock_t start, end;
    double cpu_time_used;
    pthread_t thread1, thread2;
    start = clock();
    pthread_create(&thread1, NULL, heap_sort, (void *)arr);
    pthread_create(&thread2, NULL, heap_sort, (void *)&arr[n / 2]);
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);
    merge(arr, n);
    measure_memory_usage();
    printf("Sorted array from heapsort threads: ");
    for (int i = 0; i < 10; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("Execution time: %f seconds for heapsort threads\n", cpu_time_used);
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
