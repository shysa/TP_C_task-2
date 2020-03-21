#include <time.h>
#include <stdio.h>
#include <stdlib.h>

#define ACCURACY 10

typedef struct {
    int size_array_mb;
    long avg_time;
    char lib_type[15];
    int module_sum;
} time_statistics;

long mtime();
int output_time_test_data(time_statistics statistics);

