#define SIZE 16
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

typedef struct node
{
    char *key;
    char *data;
} node_t;

typedef struct vector
{
    node_t *raw;
    size_t actual_size;
    size_t max_size;
} vector_t;

typedef struct v_iterator
{
    size_t last_idx;
} v_iterator_t;

void vector_insert(vector_t *v, char *key, char *data);
vector_t vector_init(size_t size);
vector_t vector_default_init();
void vector_destroy(vector_t *v);
node_t *vector_search(vector_t *v, char *key);
void vector_resize(vector_t *v);
void vector_delete(vector_t *v, char *key);
void vector_swap(node_t *p1, node_t *p2);
void vector_insert_when_rehash(vector_t *v, node_t *n);
v_iterator_t vector_iterator_init();
node_t *vector_iterate(vector_t *v, v_iterator_t *it);