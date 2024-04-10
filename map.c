#include "map.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

map_t map_create_default(void)
{
    return map_create(MAP_SIZE_BUCKET, MAP_SIZE);
};

map_t map_create(size_t size, size_t max_size) {
    map_t map = {NULL, size, 0, max_size};
    map.buckets = (node_t **)calloc(size, sizeof(node_t *));
    MALLOC_CHECK(map.buckets, break);
    for (size_t i = 0; i < size; i++) {
        map.buckets[i] = (node_t *)calloc(1, sizeof(node_t));
        MALLOC_CHECK(map.buckets[i], break);
        map.buckets[i]->entries = (dynamic_array_t *)malloc(sizeof(dynamic_array_t));
        MALLOC_CHECK(map.buckets[i]->entries, break);
        map.buckets[i]->entries->data = NULL;
        map.buckets[i]->entries->size = 0;
        map.buckets[i]->entries->capacity = 0;
    }
    return map;
}

void map_add(map_t *map, char *value, char *data) {
    size_t idx = compute_key(value, map->size) % map->size;
    node_t *node = map->buckets[idx];
    dynamic_array_t *entries = node->entries;

    // Cerca se la chiave esiste già nel nodo
    size_t i;
    for (i = 0; i < entries->size; i += 2) {
        if (strcmp((char *)entries->data[i], value) == 0) {
            // Chiave trovata, aggiorna il valore e restituisci
            free(entries->data[i + 1]);  // Libera il vecchio valore
            entries->data[i + 1] = strdup(data);  // Aggiorna con il nuovo valore
            return;
        }
    }

// Se la chiave non è stata trovata, aggiungila al vettore dinamico
    char *key_copy = strdup(value);
    char *data_copy = strdup(data);
    entries->data = (void **)realloc(entries->data, (entries->size + 2) * sizeof(void *));
    MALLOC_CHECK(entries->data, return);
    entries->data[entries->size] = key_copy;
    entries->data[entries->size + 1] = data_copy;
    entries->size += 2;
    node->length++;

    double load_factor = (double)(map->nelem + 1) / map->max_size;
    if (load_factor >= THRESH_BALANCE) {
        map_resize(map);
    }
}


void map_delete(map_t *map, char *value) {
    size_t idx = compute_key(value, map->size) % map->size;
    node_t *node = map->buckets[idx];
    dynamic_array_t *entries = node->entries;

    // Cerca la chiave nel vettore dinamico del nodo
    for (size_t i = 0; i < entries->size; i += 2) {
        if (strcmp((char *)entries->data[i], value) == 0) {
            // Chiave trovata, elimina la coppia chiave-valore
            free(entries->data[i]);
            free(entries->data[i + 1]);
            // Sposta gli elementi successivi nel vettore
            memmove(&entries->data[i], &entries->data[i + 2], (entries->size - i - 2) * sizeof(void *));
            entries->size -= 2;
            node->length--;
            map->nelem--;
            return;
        }
    }
}

char *map_find(map_t *map, char *value) {
    size_t idx = compute_key(value, map->size) % map->size;
    node_t *node = map->buckets[idx];
    dynamic_array_t *entries = node->entries;

    // Cerca la chiave nel vettore dinamico del nodo
    for (size_t i = 0; i < entries->size; i += 2) {
        if (strcmp((char *)entries->data[i], value) == 0) {
            // Chiave trovata, restituisci il valore associato
            return (char *)entries->data[i + 1];
        }
    }
    // Chiave non trovata
    return NULL;
}

void map_destroy(map_t *map) {
    if (map == NULL) {
        return;
    }
    for (size_t i = 0; i < map->size; i++) {
        if (map->buckets[i] != NULL) {
            dynamic_array_t *entries = map->buckets[i]->entries;
            // Libera le coppie chiave-valore nel vettore dinamico
            for (size_t j = 0; j < entries->size; j += 2) {
                free(entries->data[j]);
                free(entries->data[j + 1]);
            }
            // Libera il vettore dinamico e il nodo
            free(entries->data);
            free(map->buckets[i]->entries);
            free(map->buckets[i]);
        }
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
    return hashval |= size;
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

void map_resize(map_t *map) {
    size_t new_size = map->size * 2;
    node_t **new_buckets = (node_t **)calloc(new_size, sizeof(node_t *));
    MALLOC_CHECK(new_buckets, return);
    for (size_t i = 0; i < new_size; i++) {
        new_buckets[i] = (node_t *)calloc(1, sizeof(node_t));
        MALLOC_CHECK(new_buckets[i], return);
        new_buckets[i]->entries = (dynamic_array_t *)malloc(sizeof(dynamic_array_t));
        MALLOC_CHECK(new_buckets[i]->entries, return);
        new_buckets[i]->entries->data = NULL;
        new_buckets[i]->entries->size = 0;
        new_buckets[i]->entries->capacity = 0;
    }
    for (size_t i = 0; i < map->size; i++) {
        node_t *current = map->buckets[i];
        dynamic_array_t *entries = current->entries;
        // Trasferisci gli elementi dal vettore dinamico del nodo corrente al nuovo vettore dinamico
        for (size_t j = 0; j < entries->size; j += 2) {
            char *key = (char *)entries->data[j];
            char *value = (char *)entries->data[j + 1];
            size_t idx = compute_key(key, new_size) % new_size;
            dynamic_array_t *new_entries = new_buckets[idx]->entries;
            new_entries->data = (void **)realloc(new_entries->data, (new_entries->size + 2) * sizeof(void *));
            MALLOC_CHECK(new_entries->data, return);
            new_entries->data[new_entries->size] = strdup(key);
            new_entries->data[new_entries->size + 1] = strdup(value);
            new_entries->size += 2;
            new_buckets[idx]->length++;
        }
        // Libera il vettore dinamico del nodo corrente
        free(entries->data);
        free(entries);
        free(current);
    }
    free(map->buckets);
    map->buckets = new_buckets;
    map->size = new_size;
    map->max_size *= 2;
}

obj_t map_iterate(map_t *map, map_status_t *status) {
    if (status->last == NULL) {
        // Inizia dall'inizio della mappa
        status->bucket_n = -1;
    }
    size_t current_bucket = status->bucket_n + 1;
    while (current_bucket < map->size) {
        node_t *node = map->buckets[current_bucket];
        dynamic_array_t *entries = node->entries;
        if (entries->size > 0) {
            // Trovato un nodo non vuoto, imposta lo stato e restituisci
            status->bucket_n = current_bucket;
            status->last = entries->data[0];
            return status->last;
        }
        current_bucket++;
    }
    // Fine della mappa, imposta lo stato a NULL
    status->last = NULL;
    return status->last;
}

map_status_t init_iterator(void) {
    map_status_t ret = {-1, NULL};
    return ret;
}
