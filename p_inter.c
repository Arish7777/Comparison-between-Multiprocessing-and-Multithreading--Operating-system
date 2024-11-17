#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <pthread.h>
#include <time.h>
#include <sys/resource.h>
#include <sys/wait.h>

#define MAX_SIZE 10

void measure_memory_usage();
void measure_memory_usage() {
    struct rusage usage;

    getrusage(RUSAGE_SELF, &usage);

    printf("Memory usage: %ld KB\n", usage.ru_maxrss);
}



void interpolation_search(int arr[], int n, int x, int *pos)
{
    int low = 0;
    int high = n - 1;

    while (low <= high && x >= arr[low] && x <= arr[high]) {
        int mid = low + ((x - arr[low]) * (high - low)) / (arr[high] - arr[low]);

        if (arr[mid] == x) {
            *pos = mid;
            return;
        }

        if (arr[mid] < x) {
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }

    *pos = -1;
}

int main()
{
    int arr[MAX_SIZE] = {1, 3, 5, 7, 9, 11, 13, 15, 17, 19};
    int n = sizeof(arr) / sizeof(arr[0]);
    int x = 11;
    int pos = -1;
    int status;

    int p1[2], p2[2];
    clock_t start, end;
    double cpu_time_used;
      start = clock();
    pipe(p1);
    pipe(p2);

    if (fork() == 0) {
        close(p1[1]);
        close(p2[0]);

        int temp_pos;
        interpolation_search(arr, n/2, x, &temp_pos);
        write(p2[1], &temp_pos, sizeof(temp_pos));
        close(p2[1]);

        exit(0);
    } else {
        if (fork() == 0) {
            close(p1[0]);
            close(p2[1]);

            int temp_pos;
            interpolation_search(arr + n/2, n - n/2, x, &temp_pos);
            write(p1[1], &temp_pos, sizeof(temp_pos));
            close(p1[1]);

            exit(0);
        } else {
            close(p1[1]);
            close(p2[1]);

            wait(&status);
            if (WIFEXITED(status)) {
                int temp_pos;
                read(p2[0], &temp_pos, sizeof(temp_pos));
                if (temp_pos != -1) {
                    pos = temp_pos;
                }
            }

            wait(&status);
            if (WIFEXITED(status)) {
                int temp_pos;
                read(p1[0], &temp_pos, sizeof(temp_pos));
                if (temp_pos != -1) {
                    pos = n/2 + temp_pos;
                }
            }

            close(p1[0]);
            close(p2[0]);

            printf("Position of %d in the array is %d\n", x, pos);
        }
        
    }
    measure_memory_usage();
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("Execution time: %f seconds for interpolation search process\n", cpu_time_used);
    
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

