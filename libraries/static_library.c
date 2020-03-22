#include "lib_includes.h"

int* get_array_memory(size_t * size, size_t size_array_mb) {
    *size = (size_array_mb * 1024 * 1024) / sizeof(int);

    int *array = (int *) malloc(*size * sizeof(int));
    if (!array) {
        return ALLOCATION_ERROR;
    }

    return array;
}

int init_array(FILE * input, int* array, size_t size) {
    if (!array || !size || !input) {
        return INVALID_DATA;
    }

    bool error = false;
    for (size_t i = 0; i < size; i++) {
        if (!fscanf(input, "%d ", &array[i])) {
            error = true;
            free_array_memory(array, size);
            break;
        }
    }
    if (error) {
        return INVALID_DATA;
    }

    return SUCCESS;
}

int init_array_simple(int* array, size_t size) {
    if (!array || !size) {
        return INVALID_DATA;
    }

    for (size_t i = 0; i < size; i++) {
        array[i] = ARRAY_VALUE_FOR_SIMPLE_INIT;
    }

    return SUCCESS;
}

int count_elementary_sum(const int * array, size_t size, int * sum) {
    if (!array || !size) {
        return INVALID_DATA;
    }

    *sum = 0;

    for (size_t i = 0; i < size; i++) {
        *sum += array[i] % MODULE;
        *sum %= MODULE;
    }

    return SUCCESS;
}

int free_array_memory(int* array, size_t size) {
    if (!array || !size) {
        return ERROR_FREE_MEMORY;
    }
    free(array);

    return SUCCESS;
}