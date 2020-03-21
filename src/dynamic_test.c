#include <dlfcn.h>
#include <stdio.h>

#include "time_test_funcs.h"

#include "lib_includes.h"

int main() {
    size_t size = 0;
    int * array = NULL;

    // ===================================== FOR STATISTICS ===========================================
    long t;
    long avg_time;
    int get_sum = 0;


    // ======================================== DYNAMIC ===============================================
    // OPEN LIBRARY ===================================================================================
    void * library = dlopen("./libdynamic_lib.so", RTLD_LAZY);
    if (!library) {
        fprintf(stderr,"dlopen() error: %s\n", dlerror());
        dlclose(library);
        return -1;
    }

    // GET FUNCTIONS ==================================================================================
    void* (*get_array_memory)(size_t * size, const size_t size_array_mb);
    void* (*init_array)(FILE * input, int* massive, const size_t size);
    void* (*count_elementary_sum)(const int * massive, const size_t size, int * sum);
    void* (*free_array_memory)(int* massive, const size_t size);

    *(void**)(&get_array_memory) = dlsym(library, "get_array_memory");
    if (!get_array_memory) {
        fprintf(stderr,"%s\n", dlerror());
        dlclose(library);
        return -1;
    }

    *(void**)(&init_array)= dlsym(library, "init_array");
    if (!init_array) {
        fprintf(stderr,"%s\n", dlerror());
        dlclose(library);
        return -1;
    }

    *(void**)(&count_elementary_sum)= dlsym(library, "count_elementary_sum");
    if (!count_elementary_sum) {
        fprintf(stderr,"%s\n", dlerror());
        dlclose(library);
        return -1;
    }

    *(void**)(&free_array_memory)= dlsym(library, "free_array_memory");
    if (!free_array_memory) {
        fprintf(stderr,"%s\n", dlerror());
        dlclose(library);
        return -1;
    }

    // TESTING ========================================================================================
    for (int size_array_mb = SIZE_MB; size_array_mb < MAX_SIZE_MB; size_array_mb *= 2) {
        avg_time = 0;

        array = get_array_memory(&size, size_array_mb);
        if (!array) {
            fprintf(stderr, "Memory allocation error");
            return -1;
        }

        if (create_text_data(size)) {
            fprintf(stderr, "Can't open input file");
            return -1;
        }
        FILE * file = fopen(DEFAULT_INPUT_TXT, "r");

        if (init_array(file, array, size)) {
            fprintf(stderr, "Initialization error");
            return -1;
        }

        fclose(file);

        for (char i = 0; i < ACCURACY; i++) {
            t = mtime();
            if (count_elementary_sum(array, size, &get_sum)) {
                fprintf(stderr, "Sum calculating error");
                return -1;
            }
            t = mtime() - t;

            avg_time += t;
        }
        if (free_array_memory(array, size)) {
            fprintf(stderr, "Memory freeing error");
            return -1;
        }

        avg_time /= ACCURACY;

        time_statistics dlib_statistics = {
                .size_array_mb = size_array_mb,
                .avg_time = avg_time,
                .lib_type = "DYNAMIC LIB",
                .module_sum = get_sum
        };
        if (output_time_test_data(dlib_statistics)) {
            fprintf(stderr, "Can't print results into file");
            return -1;
        }

        printf("Dynamic library takes %ld ms for %d MB\n", avg_time, size_array_mb);
    }

    dlclose(library);

    return 0;
}