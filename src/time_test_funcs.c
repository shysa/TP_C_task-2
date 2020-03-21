#include "time_test_funcs.h"

long mtime() {
    struct timespec t;

    clock_gettime(CLOCK_REALTIME, &t);
    long mt = (long)t.tv_sec * 1000 + t.tv_nsec / 1000000;
    return mt;
}

int output_time_test_data(const time_statistics statistics) {
    FILE * file = fopen("time_statistics.txt", "a+");
    if (!file) {
        return -1;
    }

    fprintf(file, "-----------------------------------------------------\n");
    fprintf(file, "TESTED: %s\n", statistics.lib_type);
    fprintf(file, "ACCURACY: %d ITERATIONS\n", ACCURACY);
    fprintf(file, "ARRAY SIZE: %d MB\n", statistics.size_array_mb);
    fprintf(file, "AVERAGE TIME: %ld ms\n", statistics.avg_time);
    fprintf(file, "RETURNED SUM: %d\n", statistics.module_sum);
    fprintf(file, "-----------------------------------------------------\n\n");

    fclose(file);

    return 0;
}