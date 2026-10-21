/*
 * Copyright (c) 2026 Jakub Marszałek
 *
 * SPDX-License-Identifier: MIT
 *
 * This software is provided "as is", without warranty of any kind.
 * See the LICENSE file for details.
 */

/* Private includes ----------------------------------------------------------*/
#include <stdlib.h>
#include "vector.h"

/* Private defines -----------------------------------------------------------*/
#define V_INIT_SIZE      (0)
#define V_INIT_CAPACITY  (4)
#define V_CAP_MULTIPLIER (2)

/* Private types -------------------------------------------------------------*/
/**
  * @brief  Vector structure definition.
  * @note   Must be initialized with vector_create() 
  *         before using any other vector function.
  */
struct vector_t {
    int *data;
    size_t size;
    size_t capacity;
};

/* Public functions ----------------------------------------------------------*/
vector_t *vector_create(void)
{
    /* vector structure */
    vector_t *vec = malloc(sizeof(*vec));
    if (NULL == vec) {
        return NULL;
    }

    /* vector data */
    vec->data = malloc(V_INIT_CAPACITY * sizeof(int));
    if (NULL == vec->data) {
        free(vec);
        return NULL;
    }

    /* assign initial values */
    vec->size     = V_INIT_SIZE;
    vec->capacity = V_INIT_CAPACITY;

    return vec;
}

vector_status_t vector_size(const vector_t *vec, size_t *output)
{
    if (NULL == vec || NULL == output) {
        return VEC_ERR_NULL;
    }

    *output = vec->size;
    return VEC_OK;
}

vector_status_t vector_capacity(const vector_t *vec, size_t *output)
{
    if (NULL == vec || NULL == output) {
        return VEC_ERR_NULL;
    }

    *output = vec->capacity;
    return VEC_OK;
}

vector_status_t vector_push_back(vector_t *vec, int value)
{
    if (NULL == vec) {
        return VEC_ERR_NULL;
    }

    /* there is free space in vector - no reallocation */
    if (vec->size < vec->capacity) {
        vec->data[vec->size] = value;
        vec->size++;
        return VEC_OK;
    }

    // else: THERE IS NO SPACE LEFT IN VECTOR - NEEDS MEMORY REALLOCATION

    return VEC_ERR_NOMEM;
}

vector_status_t vector_get(const vector_t *vec, const size_t index, int *output)
{
    if (NULL == vec || NULL == output) {
        return VEC_ERR_NULL;
    }

    if (index >= vec->size) {
        return VEC_ERR_RANGE;
    }

    *output = vec->data[index];
    return VEC_OK;
}

void vector_destroy(vector_t *vec)
{
    if (NULL == vec) {
        return;
    }

    if (NULL != vec->data) {
        free(vec->data);
    }

    free(vec);
}