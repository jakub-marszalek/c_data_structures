#include <stdio.h>
#include "vector.h"

int main()
{
    // Create a new vector
    vector_t *vec = vector_create();
    if (NULL == vec) {
        fprintf(stderr, "Mem alloc for vector failed!\n");
        return -1;
    }

    if (vector_push_back(vec, 3) != VEC_OK) {
        printf("error\n");
    }
    if (vector_push_back(vec, 1) != VEC_OK) {
        printf("error\n");
    }
    if (vector_push_back(vec, 2) != VEC_OK) {
        printf("error\n");
    }
    if (vector_push_back(vec, 4) != VEC_OK) {
        printf("error\n");
    }
    if (vector_push_back(vec, 5) != VEC_OK) {
        printf("error\n");
    }

    int x;
    for (size_t i = 0; i < 5; i++) {
        if (vector_get(vec, i, &x) != VEC_OK) {
            printf("read error\n");
        } else {
            printf("vec->data[i]: %d\n", x);
        }
    }

    // Destroy the vector
    vector_destroy(vec);

    return 0;
}