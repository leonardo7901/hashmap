#include "map.h"
#include <stdio.h>
#include <assert.h>
#include <stdio.h>

int main(void)
{
    map_t map = map_create(MAP_SIZE_BUCKET, MAP_SIZE);
    map_add(&map, "A", "1");
    map_add(&map, "A", "1");
    char *f = map_find(&map, "A");
    printf("%s\n", f);
    map_add(&map, "A", "1");
    map_add(&map, "A", "1");
    map_add(&map, "A", "1");
    map_add(&map, "A", "2");
    f = map_find(&map, "A");
    printf("%s\n", f);
    map_add(&map, "Ax", "2");
    map_find(&map, "A");
    printf("%s\n", f);
    map_add(&map, "A", "1");
    map_add(&map, "Af", "1");
    map_add(&map, "B", "4");
    map_add(&map, "b", "5");
    map_status_t it = init_iterator();
    obj_t *key_val = NULL;
    for (;;)
    {
        key_val = map_iterate(&map, &it);
        if (key_val != NULL)
        {
            printf("KEY %s, VAL %s\n", key_val->key, key_val->data);
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
            printf("KEY %s, VAL %s\n", key_val->key, key_val->data);
        }
        else
        {
            break;
        }
    }
    map_destroy(&map);
    return 0;
}