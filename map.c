#include "map.h"
#include "vector.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

map_t map_create_default(void)
{
    return map_create(MAP_SIZE_BUCKET, MAP_SIZE);
};

map_t map_create(size_t size, size_t max_size)
{
    map_t map = {NULL, size, 0, max_size};
    map.buckets = (vector_t *)malloc(sizeof(vector_t) * size);
    MALLOC_CHECK(map.buckets, break);
    for (size_t i = 0; i < size; i++)
    {
        map.buckets[i] = vector_default_init();
    }
    return map;
}

void map_add(map_t *map, char *value, char *data)
{
    size_t idx = compute_key(value, map->size) % map->size;
    vector_t *bucket = &map->buckets[idx];
    short state = vector_ordered_insert(bucket, value, data);
    if (state == NEW)
    {
        map->nelem++;
    }
    double load_factor = (double)((map->nelem + 1) / map->max_size);
    if (load_factor >= THRESH_BALANCE)
    {
        map_resize(map);
    }
}

void map_delete(map_t *map, char *value)
{
    size_t idx = compute_key(value, map->size) % map->size;
    vector_t *bucket = &map->buckets[idx];
    if (vector_ordered_delete(bucket, value))
    {
        map->nelem--;
    }
    return;
}

char *map_find(map_t *map, char *value)
{
    size_t idx = compute_key(value, map->size) % map->size;
    vector_t *bucket = &map->buckets[idx];
    search_t position = binary_search(bucket->raw, 0, bucket->actual_size, value);
    if (position.found)
    {
        return bucket->raw[position.idx].data;
    }
    else
    {
        return NULL;
    }
}

void map_destroy(map_t *map)
{
    if (map == NULL)
    {
        return;
    }
    for (size_t i = 0; i < map->size; i++)
    {
        vector_destroy(&map->buckets[i]);
    }
    free(map->buckets);
}

size_t compute_key(char *string, size_t size)
{
    size_t hashval = 0;
    while (*string)
    {
        hashval += *string++;
        hashval += (hashval << 10);
        hashval ^= (hashval >> 6);
    }
    hashval += (hashval << 3);
    hashval ^= (hashval >> 11);
    hashval += (hashval << 15);
    return hashval ^ size;
}

void map_resize(map_t *map)
{
    size_t new_size = map->size * 2;
    vector_t *new_buckets = (vector_t *)malloc(new_size * sizeof(vector_t));
    MALLOC_CHECK(new_buckets, return);
    for (size_t i = 0; i < new_size; i++)
    {
        new_buckets[i] = vector_default_init();
    }
    for (size_t i = 0; i < map->size; i++)
    {
        vector_t *current = &map->buckets[i];
        v_iterator_t it = vector_iterator_init();
        node_t *data = NULL;
        for (;;)
        {
            data = vector_iterate(current, &it);
            if (data != NULL)
            {
                size_t idx = compute_key(data->key, new_size) % new_size;
                vector_ordered_insert(&new_buckets[idx], data->key, data->data);
            }
            else
            {
                break;
            }
        }
        free(current->raw);
    }
    free(map->buckets);
    map->buckets = new_buckets;
    map->size = new_size;
    map->max_size *= 2;
    return;
}

obj_t *map_iterate(map_t *map, map_status_t *status)
{
    obj_t *data = vector_iterate(&map->buckets[status->bucket_n], &status->v_it);
    if (data == NULL)
    {
        while (++status->bucket_n < map->size)
        {
            status->v_it = vector_iterator_init();
            data = vector_iterate(&map->buckets[status->bucket_n], &status->v_it);
            if (data != NULL)
            {
                return data;
            }
        }
        return NULL;
    }
    return data;
}

map_status_t init_iterator(void)
{
    map_status_t ret;
    ret.bucket_n = 0;
    ret.v_it = vector_iterator_init();
    return ret;
}
