#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/sysinfo.h>

#define SIZE_MB 100
#define MODULE 1024

int* get_massive_memory(size_t * size);
int init_massive(int* massive, size_t size);
int count_elementary_sum(const int * massive, size_t size);
int free_massive_memory(int* massive, size_t size);