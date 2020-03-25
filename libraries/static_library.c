#include "lib_includes.h"

int* get_array_memory(size_t size) {
    int *array = (int*) malloc(size * sizeof(int));
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
            free_array_memory(array);
            break;
        }
    }
    if (error) {
        return INVALID_DATA;
    }

    return SUCCESS;
}

long division_modulo_value(long sum, int array_value, int module) {
    if (!array_value || !module) {
        return INVALID_DATA;
    }
    sum += array_value % module;
    sum %= module;
    return sum;
}

long count_elementary_sum(const int *array, size_t size, int module) {
    if (!array || !size || !module) {
        return INVALID_DATA;
    }

    long sum = 0;
    for (size_t i = 0; i < size; i++) {
        sum = division_modulo_value(sum, array[i], module);
        if (sum < 0) {
            return INVALID_DATA;
        }
    }

    return sum;
}

int free_array_memory(int* array) {
    if (!array) {
        return ERROR_FREE_MEMORY;
    }
    free(array);

    return SUCCESS;
}

int init_array_with_same_default_value(int* array, size_t size) {
    if (!array || !size) {
        return INVALID_DATA;
    }

    for (size_t i = 0; i < size; i++) {
        array[i] = ARRAY_VALUE_FOR_SIMPLE_INIT;
    }

    return SUCCESS;
}