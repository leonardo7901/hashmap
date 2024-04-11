#ifndef MAP
#define MAP
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "vector.h"

// consider K factor as O(16) in best case as full balanced, rehash only when map is full, so thresh is 1
#define THRESH_BALANCE 1
#define MAP_SIZE 4096
#define MAP_SIZE_BUCKET 256
#define MOVE_SEMANTIC 0
#define COPY_SEMANTIC 1

typedef struct map
{

    vector_t *buckets;
    size_t size;
    size_t nelem;
    size_t max_size;

} map_t;

typedef struct map_status
{
    long bucket_n;
    v_iterator_t v_it;

} map_status_t;

typedef node_t obj_t;

map_t map_create_default(void);
map_t map_create(size_t size, size_t max_size);
void map_add(map_t *map, char *value, char *data);
char *map_find(map_t *map, char *value);
void map_destroy(map_t *map);
size_t compute_key(char *string, size_t nelem);
node_t *create_node(void);
map_status_t init_iterator(void);
obj_t *map_iterate(map_t *map, map_status_t *status);
void map_resize(map_t *map);
void map_delete(map_t *map, char *value);

#define MALLOC_CHECK(ptr, call_free)                \
    do                                              \
    {                                               \
        if ((ptr) == NULL)                          \
        {                                           \
            perror("memory error, destroying map"); \
            call_free;                              \
        }                                           \
    } while (0)

#endif
