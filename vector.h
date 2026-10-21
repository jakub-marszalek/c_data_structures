// Copyright

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef VECTOR_H
#define VECTOR_H

/* Exported typedef ----------------------------------------------------------*/
typedef struct vector_t vector_t;

/**
  * @brief Vector functions status codes enumeration.
  * @note  After invoking a vector function, make sure to 
  *        check returned status code.
  **/
typedef enum {
    VEC_OK = 0,     /* Operation executed succesfully   */
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
vector_status_t vector_push_back(vector_t *vec, int value);

/**
  * @brief   Get vector data by index.
  * @param   vec     Pointer to the vector.
  * @param   index   Index of vector data field.
  * @param   output  Address of the output variable.
  * @retval  vector_status_t  VEC_OK on success, VEC_ERR_NULL or VEC_ERR_RANGE on failure.
  */
vector_status_t vector_get(const vector_t *vec, const size_t index, int *output);

/**
  * @brief  Release the vector and its data.
  * @param  vec  Pointer to the vector to destroy.
  * @note   The pointer is invalid after this call.
  */
void vector_destroy(vector_t *vec);

#endif // VECTOR_H