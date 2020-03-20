#include "dlib_includes.h"

// -----------------------------------------------------------------------------------
typedef struct {
    size_t size;
    int offset;
    int * massive;
    int thr_sum;
} thr_part_massive;

// Подсчет суммы части массива
void* count_part_sum(void* thr_data) {
    thr_part_massive* data = (thr_part_massive*) thr_data;

    int start_thr_index = data->offset * data->size;
    data->thr_sum = 0;

    for(size_t i = start_thr_index; i < start_thr_index + data->size; i++) {
        data->thr_sum += data->massive[i] % MODULE;
        data->thr_sum %= MODULE;
    }

    return NULL;
}

void start_threads(pthread_t* threads, thr_part_massive* thr_data, int thr_count) {
    for (size_t i = 0; i < thr_count; i++) {
        pthread_create(&(threads[i]), NULL, count_part_sum, &thr_data[i]);
    }
    //ожидание выполнения всех потоков
    for(int i = 0; i < thr_count; i++) {
        pthread_join(threads[i], NULL);
    }
}

int init_threads(thr_part_massive* thr_data, const int * massive, const int thr_count, const size_t massive_size) {
    if (!thr_data || !massive || !thr_count || !massive_size) {
        return -1;
    }

    for (size_t i = 0; i < thr_count; i++) {
        thr_data[i].offset = i;
        thr_data[i].size = massive_size / thr_count;
        thr_data[i].massive = (int *) massive;
    }
    return 0;
}

pthread_t* get_memory_thr_handlers(int thr_count) {
    //память под массив идентификаторов потоков
    pthread_t* threads = (pthread_t*) malloc(thr_count * sizeof(pthread_t));
    if (!threads) {
        return NULL;
    }
    return threads;
}
thr_part_massive* get_memory_thr_data(int thr_count) {
    //кол-во потоков = кол-во структур потоковых данных
    thr_part_massive* thr_data = (thr_part_massive*) malloc(thr_count * sizeof(thr_part_massive));
    if (!thr_data) {
        return NULL;
    }
    return thr_data;
}

int free_thread_memory(pthread_t* threads) {
    if (!threads) {
        return -1;
    }
    free(threads);
    return 0;
}
int free_thrdata_memory(thr_part_massive* thr_data) {
    if (!thr_data) {
        return -1;
    }
    free(thr_data);
    return 0;
}

// -----------------------------------------------------------------------------------

int count_elementary_sum(const int * massive, const size_t size) {
    int sum = 0;

    int thr_count = get_nprocs();
    pthread_t* threads = NULL;
    thr_part_massive* thr_data = NULL;

    threads = get_memory_thr_handlers(thr_count);
    if (!threads) {
        return -1;
    }
    thr_data = get_memory_thr_data(thr_count);
    if (!thr_data) {
        return -1;
    }

    if (init_threads(thr_data, massive, thr_count, size)) {
        return -1;
    }

    start_threads(threads, thr_data, thr_count);

    for (size_t i = 0; i < thr_count; i++) {
        sum += thr_data[i].thr_sum % MODULE;
        sum %= MODULE;
    }

    if (free_thread_memory(threads)) {
        return -1;
    }
    if (free_thrdata_memory(thr_data)) {
        return -1;
    }

    return 0;
}

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
    return 0;
}

int free_massive_memory(int* massive, const size_t size) {
    if (!massive || !size) {
        return -1;
    }
    free(massive);
    return 0;
}