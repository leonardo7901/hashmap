#ifndef MAP_H
#define MAP_H
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

// Constants
#define THRESH_BALANCE 1  // Rehash only when map is full
#define MAP_SIZE 4096      // Initial bucket array size
#define MAP_SIZE_BUCKET 256 // Size of each bucket (number of nodes per bucket)

// Semantic for list_add function
#define MOVE_SEMANTIC 0
#define COPY_SEMANTIC 1

// Data structures
typedef struct list {
  char *value;
  char *data;
  struct list *next;
  struct list *prev;
} list_t;

typedef struct dynamic_array {
  void **data;
  size_t size;
  size_t capacity;
} dynamic_array_t;

typedef struct node {
  dynamic_array_t *entries;
  size_t length;
} node_t;

typedef struct map {
  node_t **buckets;
  size_t size;
  size_t nelem;
  size_t max_size;
} map_t;

typedef struct map_status {
  long bucket_n;
  void *last;
} map_status_t;

typedef void* obj_t;

// Function prototypes (use void* for obj_t for flexibility)
map_t map_create_default(void);
map_t map_create(size_t size, size_t max_size);
void map_add(map_t *map, char *value, char *data);
char *map_find(map_t *map, char *value);
void map_destroy(map_t *map);
size_t compute_key(char *string, size_t nelem);
node_t *create_node(void);
map_status_t init_iterator(void);
void* map_iterate(map_t *map, map_status_t *status);
void map_resize(map_t *map);
void list_add(list_t **list_head, list_t **list_tail, char *value, char *data, short _semantic, list_t *sentinel);
void delete_list(list_t **list_head, list_t **list_tail, list_t *sentinel);
void map_delete(map_t *map, char *value);
list_t *list_search(list_t *head, list_t *tail, char *value);
void list_free(list_t **list_head);
void array_insert(dynamic_array_t *array, void *key, void *value);

// Error handling macro
#define MALLOC_CHECK(ptr, call_free) \
  do { \
    if ((ptr) == NULL) { \
      perror("memory error, destroying map"); \
      call_free; \
    } \
  } while (0)

#endif
