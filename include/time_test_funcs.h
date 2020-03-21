#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ACCURACY 10
#define DEFAULT_ARRAY_VALUE 3
#define DEFAULT_INPUT_TXT "data_test_input.txt"

typedef struct {
    int size_array_mb;
    long avg_time;
    char lib_type[15];
    int module_sum;
} time_statistics;

long mtime();
int output_time_test_data(time_statistics statistics);
int create_text_data(int size);
