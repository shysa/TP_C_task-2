#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define ARRAY_VALUE_FOR_SIMPLE_INIT 3
// ---------------------------
#define SUCCESS 0
#define INVALID_DATA -1
#define ERROR_FREE_MEMORY -2
#define ALLOCATION_ERROR NULL
// ---------------------------

int* get_array_memory(size_t size);
int init_array(FILE *input, int *array, size_t size);
int init_array_with_same_default_value(int *array, size_t size);
long count_elementary_sum(const int *array, size_t size, int module);
int free_array_memory(int *array);