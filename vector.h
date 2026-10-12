#ifndef VECTOR_H
#define VECTOR_H



typedef struct vector_t vector_t;

typedef enum {
    VEC_OK = 0,     /* Operation executed succesfully   */
    VEC_ERR_NOMEM,  /* Memory allocation failure        */
    VEC_ERR_NULL,   /* Given a NULL pointer             */
    VEC_ERR_RANGE   /* Given index is outside the range */
} vector_status_t;


vector_t *vector_create();
vector_status_t vector_push_back(vector_t *vec, int value);
vector_status_t vector_get(const vector_t *vec, const size_t index, int *output);

void vector_destroy(vector_t *vec);

#endif // VECTOR_H