#include <stdio.h>
#include "vector.h"

int main()
{
    vector_t *vec = vector_create();
    int x;
    int y;
    int z;

    vector_push_back(vec, 2435);
    vector_push_back(vec, 1000);

    if (vector_get(vec, 0, &x) != VEC_OK) {
        printf("Error");
        return -1;
    }
    fprintf(stdout, "vec[0]: %d\n", x);

    if (vector_get(vec, 1, &y) != VEC_OK) {
        printf("Error");
        return -1;
    }
    fprintf(stdout, "vec[1]: %d\n", y);

    if (vector_get(vec, 2, &z) != VEC_OK) {
        printf("Error");
        return -1;
    }
    fprintf(stdout, "vec[2]: %d\n", z);

    vector_destroy(vec);

    return 0;
}