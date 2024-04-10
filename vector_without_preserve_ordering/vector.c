#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "vector.h"

void vector_insert_when_rehash(vector_t *v, node_t *n)
{
    memmove(&v->raw[v->actual_size++], n, sizeof(node_t));
    if (v->actual_size == v->max_size)
    {
        vector_resize(v);
    }
}

void vector_insert(vector_t *v, char *key, char *data)
{
    node_t *pointer = vector_search(v, key);
    if (pointer == NULL)
    {
        node_t new = {NULL, NULL};
        new.key = strdup(key);
        new.data = strdup(data);
        v->raw[v->actual_size++] = new;
    }
    else if (strcmp(pointer->data, data) != 0)
    {

        free(pointer->data);
        pointer->data = strdup(data);
    }
    if (v->actual_size == v->max_size)
    {
        vector_resize(v);
    }
}

vector_t vector_init(size_t size)
{
    vector_t v = {NULL, 0, size};
    v.raw = (node_t *)calloc(size, sizeof(node_t));
    return v;
}

vector_t vector_default_init()
{
    return vector_init(SIZE);
}

node_t *vector_search(vector_t *v, char *key)
{
    for (size_t i = 0; i < v->actual_size; i++)
    {
        if (strcmp(v->raw[i].key, key) == 0)
        {
            return &v->raw[i];
        }
    }
    return NULL;
}

void vector_resize(vector_t *v)
{
    v->max_size *= 2;
    v->raw = (node_t *)realloc(v->raw, sizeof(node_t) * v->max_size);
}

void vector_delete(vector_t *v, char *key)
{
    node_t *pointer = vector_search(v, key);
    if (pointer == NULL)
    {
        return;
    }
    else
    {
        vector_swap(pointer, &v->raw[--v->actual_size]);
    }
}

inline void vector_swap(node_t *p1, node_t *p2)
{
    node_t copy = *p2;
    *p2 = *p1;
    *p1 = copy;
}

void vector_destroy(vector_t *v)
{
    for (size_t i = 0; i < v->actual_size; i++)
    {
        free(v->raw[i].data);
        free(v->raw[i].key);
    }
    free(v->raw);
}

v_iterator_t vector_iterator_init()
{
    v_iterator_t it = {-1};
    return it;
}

node_t *vector_iterate(vector_t *v, v_iterator_t *it)
{
    if (++it->last_idx >= v->actual_size)
    {
        return NULL;
    }
    else
    {
        return &v->raw[it->last_idx];
    }
}