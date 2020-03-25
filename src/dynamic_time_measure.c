#include <dlfcn.h>
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

    // OPEN LIBRARY
    void *library = dlopen("./libdynamic_lib.so", RTLD_LAZY);
    if (!library) {
        fprintf(stderr,"dlopen() error: %s\n", dlerror());
        dlclose(library);
        return OPEN_LIBRARY;
    }

    // GET FUNCTIONS
    void* (*get_array_memory)(size_t size);
    void* (*init_array)(FILE *input, int *array, size_t size);
    void* (*count_elementary_sum)(const int *array, size_t size, int module);
    void* (*free_array_memory)(int *array);

    *(void**)(&get_array_memory) = dlsym(library, "get_array_memory");
    if (!get_array_memory) {
        fprintf(stderr,"%s\n", dlerror());
        dlclose(library);
        return OPEN_LIBRARY_FUNCTION;
    }

    *(void**)(&init_array)= dlsym(library, "init_array");
    if (!init_array) {
        fprintf(stderr,"%s\n", dlerror());
        dlclose(library);
        return OPEN_LIBRARY_FUNCTION;
    }

    *(void**)(&count_elementary_sum)= dlsym(library, "count_elementary_sum");
    if (!count_elementary_sum) {
        fprintf(stderr,"%s\n", dlerror());
        dlclose(library);
        return OPEN_LIBRARY_FUNCTION;
    }

    *(void**)(&free_array_memory)= dlsym(library, "free_array_memory");
    if (!free_array_memory) {
        fprintf(stderr,"%s\n", dlerror());
        dlclose(library);
        return OPEN_LIBRARY_FUNCTION;
    }


    // TESTING
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
        get_sum = (long)count_elementary_sum(array, size, module);
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
    time_statistics dlib_statistics = {
            .size_array = size,
            .avg_time = avg_time,
            .lib_type = "DYNAMIC LIB",
            .module_sum = get_sum
    };

    if (output_time_test_data(dlib_statistics)) {
        fprintf(stderr, "Can't print results into file");
        return OPEN_FILE;
    }

    printf("Dynamic library takes %ld ms for %zu elements\n", avg_time, size);

   dlclose(library);

    return SUCCESS;
}