/*
 * Copyright (c) 2026 Jakub Marszałek
 *
 * SPDX-License-Identifier: MIT
 *
 * This software is provided "as is", without warranty of any kind.
 * See the LICENSE file for details.
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef VECTOR_H
#define VECTOR_H

/* Private includes ----------------------------------------------------------*/
#include <stddef.h>

/* Exported typedef ----------------------------------------------------------*/
typedef struct vector_t vector_t;

/**
  * @brief Vector functions status codes enumeration.
  * @note  After invoking a vector function, make sure to 
  *        check returned status code.
  **/
typedef enum {
    VEC_OK = 0,     /* Operation executed succesfully   */
    VEC_ERR_EMPTY,  /* Given vector is empty            */
    VEC_ERR_NOMEM,  /* Memory allocation failure        */
    VEC_ERR_NULL,   /* Given a NULL pointer             */
    VEC_ERR_RANGE   /* Given index is outside the range */
} vector_status_t;

/* Exported functions prototypes ---------------------------------------------*/
/**
  * @brief   Create new vector.
  * @note    The caller owns the vector and must release it with vector_destroy().
  * @retval  vector_t*  Pointer to vector struct on success, NULL on failure.
  */
vector_t *vector_create(void);

/**
  * @brief   Get vector size.
  * @param   vec     Pointer to the vector.
  * @param   output  Address of the output variable.
  * @retval  vector_status_t  VEC_OK on success, VEC_ERR_NULL on failure.
  */
vector_status_t vector_size(const vector_t *vec, size_t *output);

/**
  * @brief   Get vector capacity.
  * @param   vec     Pointer to the vector.
  * @param   output  Address of the output variable.
  * @retval  vector_status_t  VEC_OK on success, VEC_ERR_NULL on failure.
  */
vector_status_t vector_capacity(const vector_t *vec, size_t *output);

/**
  * @brief   Push new element at the end of the data block.
  * @param   vec    Pointer to the vector.
  * @param   value  Value that will be stored in vector data.
  * @retval  vector_status_t  VEC_OK on success, VEC_ERR_NULL or VEC_ERR_NOMEM on failure.
  */
vector_status_t vector_push_back(vector_t *vec, const int value);

/**
  * @brief   Pop last element of the data block.
  * @param   vec     Pointer to the vector.
  * @param   output  Value that has been popped.
  * @retval  vecotr_status_t  VEC_OK on success, VEC_ERR_NULL or VEC_ERR_EMPTY on failure.
  */
vector_status_t vector_pop_back(vector_t *vec, int *output);

/**
  * @brief   Get vector data by index.
  * @param   vec     Pointer to the vector.
  * @param   index   Index of vector data field.
  * @param   output  Address of the output variable.
  * @retval  vector_status_t  VEC_OK on success, VEC_ERR_NULL or VEC_ERR_RANGE on failure.
  */
vector_status_t vector_get(const vector_t *vec, const size_t index, int *output);

/**
  * @brief   Set vector data by index.
  * @param   vec    Pointer to the vector.
  * @param   index  Index of vector data field to be set.
  * @param   value  Value to be set at given index.
  * @note    This function updates the value of already existing element. Function does NOT
  *          allocate new space in memory or append elements; index must be within the bounds
  *          of the vector size (0 to size - 1).
  * @retval  vector_status_t  VEC_OK on success, VEC_ERR_NULL or VEC_ERR_RANGE on failure.
  */
vector_status_t vector_set(vector_t *vec, const size_t index, const int value);

/**
  * @brief   Insert new value at given index.
  * @param   vec    Pointer to the vector.
  * @param   index  Position of the new element, in range 0 to size (inclusive).
  * @param   value  Value to be inserted.
  * @note    Elements from index to the end are shifted one position to the right.
  *          Inserting at index equal to size appends the value (same as vector_push_back()).
  *          May reallocate the data block. On failure the vector is left unchanged.
  * @retval  vector_status_t  VEC_OK on success, VEC_ERR_NULL, VEC_ERR_RANGE or VEC_ERR_NOMEM
  *                           on failure.
  */
vector_status_t vector_insert(vector_t *vec, const size_t index, const int value);

/**
  * @brief   Clear vector data.
  * @param   vec  Pointer to the vector.
  * @note    Sets size to 0. This function does NOT change vector capacity.
  *          Clearing an already empty vector is not an error.
  * @retval  vector_status_t  VEC_OK on success, VEC_ERR_NULL on failure.
  */
vector_status_t vector_clear(vector_t *vec);

/**
  * @brief  Release the vector and its data.
  * @param  vec  Pointer to the vector to destroy.
  * @note   The pointer is invalid after this call. The caller is responsible
  *         for setting it to NULL. Calling this function again with the same
  *         non-NULL pointer is undefined behavior; calling it with NULL is safe.
  */
void vector_destroy(vector_t *vec);

#endif // VECTOR_H