#include "lib_includes.h"

int* get_array_memory(size_t * size, const size_t size_array_mb) {
    *size = (size_array_mb * 1024 * 1024) / sizeof(int);

    int *array = (int *) malloc(*size * sizeof(int));
    if (!array) {
        return NULL;
    }

    return array;
}

int init_array(int* array, size_t size) {
    if (!array || !size) {
        return -1;
    }

    for (size_t i = 0; i < size; i++) {
        array[i] = 2;
    }
    return 0;
}

int count_elementary_sum(const int * array, const size_t size, int * sum) {
    if (!array || !size) {
        return -1;
    }

    *sum = 0;

    for (size_t i = 0; i < size; i++) {
        *sum += array[i] % MODULE;
        *sum %= MODULE;
    }
    return 0;
}

int free_array_memory(int* array, size_t size) {
    if (!array || !size) {
        return -1;
    }
    free(array);
    return 0;
}