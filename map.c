#include "map.h"
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
    map.buckets = (node_t **)calloc(size, sizeof(node_t *));
    MALLOC_CHECK(map.buckets, break);
    for (size_t i = 0; i < size; i++)
    {
        map.buckets[i] = (node_t *)calloc(1, sizeof(node_t));
        MALLOC_CHECK(map.buckets[i], break);
    }
    return map;
}

void map_add(map_t *map, char *value, char *data)
{
    size_t idx = compute_key(value, map->size) % map->size;
    node_t *node = map->buckets[idx];
    list_t **head = &node->head;
    list_t **tail = &node->tail;
    list_t *element = list_search(*head, *tail, value);
    if (element == NULL)
    {
        list_add(head, tail, value, data, COPY_SEMANTIC, NULL);
        map->nelem++;
        node->length++;
    }
    else
    {
        free(element->data);
        element->data = strdup(data);
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
    node_t *node = map->buckets[idx];
    list_t **head = &node->head;
    list_t **tail = &node->tail;
    list_t *element = list_search(*head, *tail, value);
    if (element != NULL)
    {
        delete_list(head, tail, element);
        free(element);
        map->nelem--;
        node->length--;
    }
    return;
}

char *map_find(map_t *map, char *value)
{
    size_t idx = compute_key(value, map->size) % map->size;
    node_t *node = map->buckets[idx];
    list_t *head = node->head;
    list_t *tail = node->tail;
    list_t *element = list_search(head, tail, value);
    if (element == NULL)
    {
        return NULL;
    }
    else
    {
        return element->data;
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
        if (map->buckets[i] != NULL)
        {
            list_free(&map->buckets[i]->head);
        }
        free(map->buckets[i]);
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

list_t *create_list(char *value, char *data)
{
    list_t *list = (list_t *)malloc(sizeof(list_t));
    MALLOC_CHECK(list, return NULL);
    list->value = strdup(value);
    list->data = strdup(data);
    list->next = NULL;
    list->prev = NULL;
    return list;
}

node_t *create_node(void)
{
    return (node_t *)calloc(1, sizeof(node_t));
}

void list_add(list_t **list_head, list_t **list_tail, char *value, char *data, short _semantic, list_t *sentinel)
{
    list_t *node = NULL;
    if (_semantic == COPY_SEMANTIC)
    {
        node = create_list(value, data);
    }
    else if (_semantic == MOVE_SEMANTIC)
    {
        node = sentinel;
    }
    node->next = *list_head;
    if (*list_head)
    {
        (*list_head)->prev = node;
    }
    else
    {
        *list_tail = node;
    }
    *list_head = node;
    return;
}

inline list_t *check_value(list_t *up, list_t *down, char *value)
{
    if (strcmp(up->value, value) == 0)
    {
        return up;
    }
    else if (strcmp(down->value, value) == 0)
    {
        return down;
    }
    return NULL;
}

list_t *list_search(list_t *list_head, list_t *list_tail, char *value)
{
    if (list_head == NULL || list_tail == NULL)
    {
        return NULL;
    }
    list_t *runner_up = list_head;
    list_t *runner_down = list_tail;
    list_t *element = NULL;
    while (runner_up != runner_down)
    {
        element = check_value(runner_up, runner_down, value);
        if (element)
        {
            return element;
        }
        if (runner_up->next != runner_down)
        {
            runner_down = runner_down->prev;
        }
        runner_up = runner_up->next;
    }
    return check_value(runner_up, runner_down, value);
}

void delete_list(list_t **list_head, list_t **list_tail, list_t *sentinel)
{
    list_t *next = sentinel->next;
    list_t *prev = sentinel->prev;
    if (next)
    {
        next->prev = prev;
    }
    else
    {
        *list_tail = prev;
    }
    if (prev)
    {
        prev->next = next;
    }
    else
    {
        *list_head = next;
    }
    if (!next && !prev)
    {
        *list_head = NULL;
        *list_tail = NULL;
    }
    free(sentinel->value);
    free(sentinel->data);
    return;
}

void list_free(list_t **list_head)
{
    list_t *runner = *list_head;
    list_t *save = NULL;
    while (runner != NULL)
    {
        save = runner->next;
        free(runner->data);
        free(runner->value);
        free(runner);
        runner = save;
    }
}

void map_resize(map_t *map)
{
    size_t new_size = map->size * 2;
    node_t **new_buckets = (node_t **)calloc(new_size, sizeof(node_t *));
    MALLOC_CHECK(new_buckets, return);
    for (size_t i = 0; i < new_size; i++)
    {
        new_buckets[i] = (node_t *)calloc(1, sizeof(node_t));
        MALLOC_CHECK(new_buckets[i], return);
    }
    for (size_t i = 0; i < map->size; i++)
    {
        node_t *current = map->buckets[i];
        list_t *head = current->head;
        list_t *next = NULL;
        list_t *save_next = NULL;
        while (head != NULL)
        {
            save_next = head->next;
            size_t idx = compute_key(head->value, new_size) % new_size;
            list_add(&new_buckets[idx]->head, &new_buckets[idx]->tail, NULL, NULL, MOVE_SEMANTIC, head);
            new_buckets[idx]->length++;
            head = save_next;
        }
        free(map->buckets[i]);
    }
    free(map->buckets);
    map->buckets = new_buckets;
    map->size = new_size;
    map->max_size *= 2;
    return;
}

obj_t *map_iterate(map_t *map, map_status_t *status)
{
    if (status->last == NULL || status->last->next == NULL)
    {
        for (size_t i = status->bucket_n + 1; i < map->size; i++)
        {
            if (map->buckets[i]->head)
            {
                status->last = map->buckets[i]->head;
                status->bucket_n = i;
                return status->last;
            }
        }
        return NULL;
    }
    else
    {
        status->last = status->last->next;
        return status->last;
    }
    return NULL;
}

map_status_t init_iterator(void)
{
    map_status_t ret = {-1, NULL};
    return ret;
}
