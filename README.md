# Comparison-between-Multiprocessing-and-Multithreading--Operating-system
Comparison between Multiprocessing and Multithreading via Sorting and Searching

The purpose of this project is to differentiate between
multithreading and multiprocessing, by measuring the memory
and time taken to complete various sorting and searching
algorithms using C language on Ubuntu.

# Algorithms
The following algorithms will be utilized:
1. Binary Search
2. Interpolation Search
3. Heap Sort
4. Quick Sort
5. Merge Sort

# Conclusion
The summary of our tests proves the following:
1. Multithreading is faster when using searching algorithms,
while consuming the same memory. (Binary search is an
anomaly because we were printing the result of each iteration
individually, and then adding the sum, producing more
overhead).
2. Multiprocessing provides results much quicker when it
comes to sorting algorithms.
3. Multiprocessing uses less memory than multithreading for
both sorting and searching algorithms.

# About Report
Results are shown in the report 
