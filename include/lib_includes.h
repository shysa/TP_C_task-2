#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define SIZE_MB 100
#define MAX_SIZE_MB 900
#define MODULE 1024
#define ARRAY_VALUE_FOR_SIMPLE_INIT 3
// ---------------------------
#define SUCCESS 0
#define INVALID_DATA -1
#define ERROR_FREE_MEMORY -2
#define ALLOCATION_ERROR NULL
// ---------------------------

int* get_array_memory(size_t * size, size_t size_array_mb);
int init_array(FILE * input, int* array, size_t size);
int init_array_simple(int* array, size_t size);
int count_elementary_sum(const int * array, size_t size, int * sum);
int free_array_memory(int* array, size_t size);