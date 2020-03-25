#include <stdio.h>

#include "time_test_funcs.h"

#include "lib_includes.h"

int main(int argc, char **argv) {
    size_t size = 0;
    int *array = NULL;
    int module = DEFAULT_MODULE;

    long t = 0;
    long avg_time = 0;
    long get_sum = 0;

    if (set_size_parameter(argc, (const char **)argv, &size)) {
        fprintf(stderr, "Wrong size parameter");
        return INVALID_SIZE_PARAMETER;
    }

    array = get_array_memory(size);
    if (!array) {
        fprintf(stderr, "Memory allocation error");
        return ALLOCATION_MEMORY;
    }

    if (create_text_data(size)) {
        fprintf(stderr, "Can't open input file");
        return OPEN_FILE;
    }
    FILE * file = fopen(DEFAULT_INPUT_TXT, "r");
    if (!file) {
        fprintf(stderr, "Can't open input file");
        return OPEN_FILE;
    }

    if (init_array(file, array, size)) {
        fprintf(stderr, "Initialization error");
        return INITIALIZATION_ARRAY;
    }

    fclose(file);

    bool error = false;
    for (char i = 0; i < MEASURE_TIME_ITERATIONS; i++) {
        t = mtime();
        get_sum = count_elementary_sum(array, size, module);
        if (get_sum < 0) {
            error = true;
            fprintf(stderr, "Sum calculating error");
            break;
        }
        t = mtime() - t;
        avg_time += t;
    }
    if (error) {
        return COUNT_ELEMENTARY_SUM;
    }

    if (free_array_memory(array)) {
        fprintf(stderr, "Memory freeing error");
        return MEMORY_FREE;
    }

    avg_time /= MEASURE_TIME_ITERATIONS;
    time_statistics lib_statistics = {
            .size_array = size,
            .avg_time = avg_time,
            .lib_type = "STATIC LIB",
            .module_sum = get_sum
    };

    if (output_time_test_data(lib_statistics)) {
        fprintf(stderr, "Can't print results into file");
        return OPEN_FILE;
    }

    printf("Static library takes %ld ms for %zu elements\n", avg_time, size);


    return SUCCESS;
}