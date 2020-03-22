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
        return ERROR_OPEN_OUTPUT_TEST_FILE;
    }

    fprintf(file, "-----------------------------------------------------\n");
    fprintf(file, "TESTED: %s\n", statistics.lib_type);
    fprintf(file, "ACCURACY: %d ITERATIONS\n", ACCURACY);
    fprintf(file, "ARRAY SIZE: %d MB\n", statistics.size_array_mb);
    fprintf(file, "AVERAGE TIME: %ld ms\n", statistics.avg_time);
    fprintf(file, "RETURNED SUM: %d\n", statistics.module_sum);
    fprintf(file, "-----------------------------------------------------\n\n");

    fclose(file);

    return SUCCESS;
}

int create_text_data(int size) {
    if (!size) {
        return INVALID_DATA;
    }
    FILE * file = fopen(DEFAULT_INPUT_TXT, "w+");
    if (!file) {
        return ERROR_OPEN_INPUT_TEST_FILE;
    }

    for (int i = 0; i < size; i++) {
        fprintf(file, "%d ", DEFAULT_ARRAY_VALUE_FOR_TEST);
    }
    fclose(file);

    return SUCCESS;
}