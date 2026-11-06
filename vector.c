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
#include <stdint.h>
#include <string.h>
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

/* Private functions ---------------------------------------------------------*/
/**
  * @brief   Grow the vector data block by doubling its capacity.
  * @param   vec  Pointer to the vector.
  * @note    Grows unconditionally. The caller decides when to call it. After success, 
  *          capacity is doubled. After failure, vector is untouched.
  * @retval  vector_status_t  VEC_OK on success, VEC_ERR_NULL or VEC_ERR_NOMEM on failure.
  */
static vector_status_t vector_grow(vector_t *vec)
{
    if (NULL == vec) {
        return VEC_ERR_NULL;
    }

    if (vec->capacity > SIZE_MAX / sizeof(int) / V_CAP_MULTIPLIER) {
        return VEC_ERR_NOMEM;
    }

    int *temp = realloc(vec->data, vec->capacity * sizeof(int) * V_CAP_MULTIPLIER);
    if (NULL == temp) {
        return VEC_ERR_NOMEM;
    }

    /* assign data to the new pointer and increase capacity */
    vec->data = temp;
    vec->capacity *= V_CAP_MULTIPLIER;
    
    return VEC_OK;
}

/* Public functions ----------------------------------------------------------*/
vector_t *vector_create(void)
{
    vector_t *vec = malloc(sizeof(*vec));
    if (NULL == vec) {
        return NULL;
    }

    vec->data = malloc(V_INIT_CAPACITY * sizeof(int));
    if (NULL == vec->data) {
        free(vec);
        return NULL;
    }

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

    /* no more space in vector - reallocation needed */
    if (vec->size >= vec->capacity) {
        vector_status_t status = vector_grow(vec);
        if (status != VEC_OK) {
            return status;
        }
    }

    vec->data[vec->size] = value;
    vec->size++;

    return VEC_OK;
}

vector_status_t vector_pop_back(vector_t *vec, int *output)
{
    if (NULL == vec || NULL == output) {
        return VEC_ERR_NULL;
    }

    if (vec->size == 0) {
        return VEC_ERR_EMPTY;
    }

    *output = vec->data[vec->size - 1];
    vec->size--;

    return VEC_OK;
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

vector_status_t vector_set(vector_t *vec, const size_t index, const int value)
{
    if (NULL == vec) {
        return VEC_ERR_NULL;
    }

    if (index >= vec->size) {
        return VEC_ERR_RANGE;
    }

    vec->data[index] = value;
    return VEC_OK;
}

vector_status_t vector_insert(vector_t *vec, const size_t index, const int value)
{
    if (NULL == vec) {
        return VEC_ERR_NULL;
    }

    if (index > vec->size) {
        return VEC_ERR_RANGE;
    }

    /* no more space in vector - reallocation needed */
    if (vec->size >= vec->capacity) {
        vector_status_t status = vector_grow(vec);
        if (status != VEC_OK) {
            return status;
        }
    }

    if (index != vec->size) {
        memmove(&vec->data[index + 1], &vec->data[index], sizeof(int) * (vec->size - index));
    }
    vec->data[index] = value;
    vec->size++;

    return VEC_OK;
}

vector_status_t vector_clear(vector_t *vec)
{
    if (NULL == vec) {
        return VEC_ERR_NULL;
    }

    vec->size = 0;    
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