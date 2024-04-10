#include "vector.h"
#include <stdio.h>

int main()
{
    vector_t v = vector_default_init();
    vector_insert(&v, "A", "1");
    vector_insert(&v, "A", "1");
    vector_insert(&v, "A", "1");
    vector_insert(&v, "A", "1");
    vector_insert(&v, "A", "1");
    vector_insert(&v, "B", "2");
    vector_insert(&v, "c", "2");
    vector_insert(&v, "d", "2");
    vector_insert(&v, "e", "2");
    vector_insert(&v, "f", "2");
    vector_insert(&v, "g", "2");
    vector_insert(&v, "h", "2");
    vector_insert(&v, "i", "2");
    vector_insert(&v, "l", "2");
    vector_insert(&v, "m", "2");
    vector_insert(&v, "n", "2");
    vector_insert(&v, "o", "2");
    vector_insert(&v, "p", "2");
    vector_insert(&v, "q", "2");
    vector_insert(&v, "r", "2");
    vector_insert(&v, "s", "2");
    vector_insert(&v, "t", "2");
    vector_insert(&v, "u", "2");
    node_t *node = vector_search(&v, "l");
    printf("%s\n", node->data);
    vector_delete(&v, "l");
    node = vector_search(&v, "l");
    if (node == NULL)
    {
        printf("%s\n", "deleted");
    }
    vector_insert(&v, "P", "2");
    node = vector_search(&v, "P");
    if (node != NULL)
    {
        printf("%s\n", node->data);
    }
    v_iterator_t it = vector_iterator_init();
    for (;;)
    {
        node_t *obj = vector_iterate(&v, &it);
        if (obj != NULL)
        {
            printf("KEY %s, %s VALUE\n", obj->key, obj->data);
        }
        else{
            break;
        }
    }
    vector_destroy(&v);
    return 0;
}