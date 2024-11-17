#!/bin/bash


# Compile and execute t_binary.c
echo "Compiling and executing t_binary.c ..."
gcc -o t_binary t_binary.c -lpthread
./t_binary

# Compile and execute p_binary.c
echo "Compiling and executing p_binary.c ..."
gcc -o p_binary p_binary.c
./p_binary



# Compile and execute t_inter.c
echo "Compiling and executing t_inter.c ..."
gcc -o t_inter t_inter.c -lpthread
./t_inter

# Compile and execute p_inter.c
echo "Compiling and executing p_inter.c ..."
gcc -o p_inter p_inter.c
./p_inter


# Compile and execute t_heapsort.c
echo "Compiling and executing t_heapsort.c ..."
gcc -o t_heapsort t_heapsort.c -lpthread
./t_heapsort

# Compile and execute p_heapsort.c
echo "Compiling and executing p_heapsort.c ..."
gcc -o p_heapsort p_heapsort.c
./p_heapsort


# Compile and execute t_quicksort.c
echo "Compiling and executing t_quicksort.c ..."
gcc -o t_quicksort t_quicksort.c -lpthread
./t_quicksort

# Compile and execute p_quicksort.c
echo "Compiling and executing p_quicksort.c ..."
gcc -o p_quicksort p_quicksort.c
./p_quicksort

# Compile and execute t_mergesort.c
echo "Compiling and executing t_mergesort.c ..."
gcc -o t_mergesort t_mergesort.c -lpthread
./t_mergesort

# Compile and execute p_mergesort.c
echo "Compiling and executing p_mergesort.c ..."
gcc -o p_mergesort p_mergesort.c
./p_mergesort


