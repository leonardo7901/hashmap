#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "vector.h"
#include <stdio.h>

vector_t vector_init(size_t size)
{
    vector_t v = {NULL, 0, size};
    v.raw = (node_t *)malloc(size * sizeof(node_t));
    return v;
}

vector_t vector_default_init()
{
    return vector_init(SIZE);
}

void vector_resize(vector_t *v)
{
    v->max_size *= 2;
    v->raw = (node_t *)realloc(v->raw, sizeof(node_t) * v->max_size);
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

search_t binary_search(node_t *v, long left, long right, char *key)
{
    search_t ret = {false, 0};
    while (left <= right)
    {
        int mid = left + (right - left) / 2;
        if (v[mid].key == NULL)
        {
            ret.idx = mid;
            return ret;
        }
        int cmp = strcmp(v[mid].key, key);
        if (cmp < 0)
        {
            left = mid + 1; // Search in the right half
        }
        else if (cmp > 0)
        {
            right = mid - 1; // Search in the left half
        }
        else
        {
            ret.found = true;
            ret.idx = mid;
            return ret;
        }
    }
    ret.found = false;
    ret.idx = left;
    return ret; // Key not found, return the position for insertion
}

short vector_ordered_insert(vector_t *v, char *key, char *data)
{
    short state = BOGUS;
    if (v->actual_size >= v->max_size)
    {
        vector_resize(v);
    }
    search_t position = binary_search(v->raw, 0, v->actual_size - 1, key);
    if (position.found == false)
    {
        memmove(&v->raw[position.idx + 1], &v->raw[position.idx], (v->actual_size - position.idx) * sizeof(node_t));
        v->raw[position.idx].key = strdup(key);
        v->raw[position.idx].data = strdup(data);
        v->actual_size++;
        state = NEW;
    }
    else
    {
        if (strcmp(v->raw[position.idx].data, data) != 0)
        {
            free(v->raw[position.idx].data);
            v->raw[position.idx].data = strdup(data);
            state = PRESENT;
        }
    }
    return state;
}

short vector_ordered_delete(vector_t *v, char *key)
{
    search_t position = binary_search(v->raw, 0, v->actual_size - 1, key);
    if (position.found)
    {
        memmove(&v->raw[position.idx], &v->raw[position.idx + 1], (v->actual_size - position.idx - 1) * sizeof(node_t));
        v->actual_size--;
        return 0;
    }
    else
    {
        return 1;
    }
}