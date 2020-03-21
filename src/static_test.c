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

    // ========================================= STATIC ===============================================
    for (int size_array_mb = SIZE_MB; size_array_mb < MAX_SIZE_MB; size_array_mb *= 2) {
        avg_time = 0;

        array = get_array_memory(&size, size_array_mb);
        if (!array) {
            fprintf(stderr, "Memory allocation error");
            return -1;
        }
        if (init_array(array, size)) {
            fprintf(stderr, "Initialization error");
            return -1;
        }

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

        time_statistics lib_statistics = {
                .size_array_mb = size_array_mb,
                .avg_time = avg_time,
                .lib_type = "STATIC LIB",
                .module_sum = get_sum
        };

        if (output_time_test_data(lib_statistics)) {
            fprintf(stderr, "Can't print results into file");
            return -1;
        }

        printf("Static library takes %ld ms for %d MB\n", avg_time, size_array_mb);
    }

    return 0;
}