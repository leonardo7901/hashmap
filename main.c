#include "map.h"
#include <stdio.h>
#include <assert.h>

int main(void)
{
    map_t map = map_create(MAP_SIZE_BUCKET, MAP_SIZE);
    map_add(&map, "A", "1");
    map_add(&map, "A", "1");
    map_add(&map, "A", "1");
    map_add(&map, "A", "1");
    map_add(&map, "A", "1");
    map_add(&map, "A", "2");
    assert(map.nelem == 1);
    map_add(&map, "Ax", "2");
    map_add(&map, "A", "1");
    map_delete(&map, "B");
    map_add(&map, "Af", "1");
    map_add(&map, "B", "4");
    map_add(&map, "b", "5");
    map_delete(&map, "B");
    assert(map.nelem == 4);
    map_status_t it = init_iterator();
    obj_t *key_val = NULL;
    for (;;)
    {
        key_val = map_iterate(&map, &it);
        if (key_val != NULL)
        {
            printf("KEY %s, VAL %s\n", key_val->value, key_val->data);
        }
        else
        {
            break;
        }
    }
    map_resize(&map);
    printf("\n");
    it = init_iterator();
    key_val = NULL;
    for (;;)
    {
        key_val = map_iterate(&map, &it);
        if (key_val != NULL)
        {
            printf("KEY %s, VAL %s\n", key_val->value, key_val->data);
        }
        else
        {
            break;
        }
    }
    assert(map.nelem == 4);
    assert(map.size == MAP_SIZE_BUCKET * 2);
    char *x = map_find(&map, "B");
    assert(x == NULL);
    x = map_find(&map, "A");
    assert(strcmp(x, "1") == 0);
    x = map_find(&map, "Ax");
    assert(strcmp(x, "2") == 0);
    map_destroy(&map);
    return 0;
}