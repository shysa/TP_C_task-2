#include <stdio.h>
#include <stdlib.h>

#define SIZE_MB 100
#define MAX_SIZE_MB 900
#define MODULE 1024

int* get_array_memory(size_t * size, size_t size_array_mb);
int init_array(int* array, size_t size);
int count_elementary_sum(const int * array, size_t size, int * sum);
int free_array_memory(int* array, size_t size);