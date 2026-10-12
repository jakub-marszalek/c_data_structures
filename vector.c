#include <stdlib.h>
#include "vector.h"

#define V_INIT_SIZE     (0)
#define V_INIT_CAPACITY (4)

struct vector_t {
    int *data;
    size_t size;
    size_t capacity;
};

vector_t *vector_create(void)
{
    /* Allocate memory for structure pointer */
    vector_t *vec = malloc(sizeof(*vec));
    if (NULL == vec) return NULL;

    /* Allocate memory for vector data */
    vec->data = malloc(V_INIT_CAPACITY * sizeof(int));
    if (NULL == vec->data) {
        free(vec);
        return NULL;
    }

    /* Assign default values to the structure fields */
    vec->size     = V_INIT_SIZE;
    vec->capacity = V_INIT_CAPACITY;

    return vec;
}

vector_status_t vector_push_back(vector_t *vec, int value)
{
    if (vec->size < vec->capacity) {
        vec->data[vec->size] = value;
        vec->size++;
        return VEC_OK; 
    } // else!!!!!!
    
    return VEC_ERR_NOMEM;
}

vector_status_t vector_get(const vector_t *vec, const size_t index, int *output)
{
    if (NULL == vec || NULL == output) {
        return VEC_ERR_NULL;
    }
    if (index > vec->size - 1) {
        return VEC_ERR_RANGE;
    }

    *output = vec->data[index];
    return VEC_OK;
}

void vector_destroy(vector_t *vec)
{
    if (NULL != vec->data) {
        free(vec->data);
    }
    if (NULL != vec) {
        free(vec);
    }
}