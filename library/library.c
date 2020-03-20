#include "lib_includes.h"

int* get_massive_memory(size_t * size) {
    *size = (SIZE_MB * 1024 * 1024) / sizeof(int);

    int *massive = (int *) malloc(*size * sizeof(int));
    if (!massive) {
        return NULL;
    }

    return massive;
}

int init_massive(int* massive, const size_t size) {
    if (!massive || !size) {
        return -1;
    }

    for (size_t i = 0; i < size; i++) {
        massive[i] = rand() % 1025;
    }
    printf("\n");
    return 0;
}

int count_elementary_sum(const int * massive, const size_t size) {
    if (!massive || !size) {
        return -1;
    }

    int sum = 0;

    for (size_t i = 0; i < size; i++) {
        sum += massive[i] % MODULE;
        sum %= MODULE;
    }

    return 0;
}

int free_massive_memory(int* massive, const size_t size) {
    if (!massive || !size) {
        return -1;
    }
    free(massive);
    return 0;
}