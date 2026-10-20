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
vector_t *vector_create(void);
vector_status_t vector_size(const vector_t *vec, size_t *output);
vector_status_t vector_capacity(const vector_t *vec, size_t *output);
vector_status_t vector_push_back(vector_t *vec, int value);
vector_status_t vector_get(const vector_t *vec, const size_t index, int *output);
void vector_destroy(vector_t *vec);


#endif // VECTOR_H