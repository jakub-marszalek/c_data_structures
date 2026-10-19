// Copyright

/* Private includes ----------------------------------------------------------*/
#include <stdio.h>
#include <assert.h>
#include <limits.h>
#include "vector.h"

/* Private defines -----------------------------------------------------------*/
#define INIT_RES_VALUE_INT  (-999)
#define INIT_SIZE_T         (99999)
#define MANY_TRIES_SIZE_T   (100000)

/* Create destroy tests ------------------------------------------------------*/
/**
  * @brief  Test vector create and destroy functions.
  */
static void test_many_create_destroy(void)
{
    for (size_t i = 0; i < MANY_TRIES_SIZE_T; i++) {
        vector_t *vec = vector_create();
        assert(NULL != vec);
        
        /* check vector size */
        vector_status_t status = VEC_ERR_NOMEM;
        size_t res = INIT_SIZE_T;
    
        status = vector_size(vec, &res);
        assert(status == VEC_OK);
        assert(res == 0);
        
        vector_destroy(vec);
    }
}

/**
  * @brief  Test vector get size from null and write to null address.
  */
static void test_null_vector_size(void)
{
    vector_status_t status = VEC_ERR_NOMEM;
    size_t res = INIT_SIZE_T;

    /* try to read from null vector */
    status = vector_size(NULL, &res);
    assert(status == VEC_ERR_NULL);
    assert(res == INIT_SIZE_T);

    /* create valid vector */
    vector_t *vec = vector_create();
    assert(NULL != vec);
    
    status = vector_size(vec, &res);
    assert(status == VEC_OK);
    assert(res == 0);

    /* try to write to null address */
    res = INIT_SIZE_T;
    status = vector_size(vec, NULL);
    assert(status == VEC_ERR_NULL);
    assert(res == INIT_SIZE_T);

    status = vector_size(vec, &res);
    assert(status == VEC_OK);
    assert(res == 0);
    
    /* free the vector */
    vector_destroy(vec);
}

/**
  * @brief  Test vector size from empty vector.
  */
static void test_vector_size_from_empty(void)
{
    /* create valid vector */
    vector_t *vec = vector_create();
    assert(NULL != vec);

    size_t res = INIT_SIZE_T;
    vector_status_t status = VEC_ERR_NOMEM;

    /* should always return 0 */
    for (size_t i = 0; i < MANY_TRIES_SIZE_T; i++) {
        status = vector_size(vec, &res);
        assert(status == VEC_OK);
        assert(res == 0);
    }

    /* free the vector */
    vector_destroy(vec);
}

/**
  * @brief  Test vector size after many push/get failures.
*/
static void test_vector_size_after_pushes(void)
{
    /* initialize vector */
    vector_t *vec = vector_create();
    assert(NULL != vec);
        
    /* check vector size */
    vector_status_t status = VEC_ERR_NOMEM;
    size_t res = INIT_SIZE_T;
    
    status = vector_size(vec, &res);
    assert(status == VEC_OK);
    assert(res == 0);

    /* fill vector with values and read size changes */
    res = 0;
    size_t counter = 0;
    for (size_t i = 0; i < 4; i++) {
        status = vector_push_back(vec, 5);
        assert(status == VEC_OK);

        counter++;
        status = vector_size(vec, &res);
        assert(status == VEC_OK);
        assert(res == counter);
    }

    /* try to read size after many push/get failures */
    for (size_t i = 0; i < MANY_TRIES_SIZE_T; i++) {
        status = vector_push_back(vec, 78);
        assert(status == VEC_ERR_NOMEM);

        int value = INIT_RES_VALUE_INT;
        status = vector_get(vec, 98890, &value);
        assert(status == VEC_ERR_RANGE);
        assert(value == INIT_RES_VALUE_INT);

        status = vector_size(vec, &res);
        assert(status == VEC_OK);
        assert(res == counter);
    }

    /* free the vector */
    vector_destroy(vec);
}

/**
  * @brief  Test destroy null vector.
*/
static void test_vector_null_destroy(void)
{
    vector_destroy(NULL);
}

/* Vector push tests ---------------------------------------------------------*/
/**
  * @brief  Test push to a null vector.
  */
static void test_null_vector_push(void)
{
    vector_status_t status = vector_push_back(NULL, 43);
    assert(status == VEC_ERR_NULL); 
}

/**
  * @brief  Test push one element scenario.
  */
static void test_vector_push_one_element(void)
{
    int test_value = 56;
    vector_status_t status = VEC_ERR_NOMEM;
    size_t size = INIT_SIZE_T;

    /* create empty vector */
    vector_t *vec = vector_create();
    assert(NULL != vec);

    /* check vector initial size */
    status = vector_size(vec, &size);
    assert(status == VEC_OK);
    assert(size == 0);

    /* push one element */
    status = vector_push_back(vec, test_value);
    assert(status == VEC_OK);
    
    /* check if pushed element is correct */
    int res = INIT_RES_VALUE_INT;
    assert(vector_get(vec, 0, &res) == VEC_OK);
    assert(res == test_value);

    /* check vector size after one push */
    status = vector_size(vec, &size);
    assert(status == VEC_OK);
    assert(size == 1);

    /* check indexes outside of range */
    assert(vector_get(vec, 1, &res) == VEC_ERR_RANGE);
    assert(vector_get(vec, 2, &res) == VEC_ERR_RANGE);

    /* free the vector */
    vector_destroy(vec);
}

/**
  * @brief  Test push elements to vector until it is full.  
  */
static void test_vector_push_until_full(void)
{
    vector_t *vec = vector_create();
    assert(NULL != vec);

    /* fill the vector with test values (initial capacity is 4) */
    int test_values[4] = {46, 12, 9, -4};
    
    vector_status_t status = VEC_ERR_NOMEM;
    for (int i = 0; i < 4; i++) {
        status = vector_push_back(vec, test_values[i]);
        assert(status == VEC_OK);
    }

    /* try to overflow the vector */
    status = vector_push_back(vec, test_values[0]);
    assert(status == VEC_ERR_NOMEM);

    status = vector_push_back(vec, test_values[0]);
    assert(status == VEC_ERR_NOMEM);

    /* read values from the vector */
    int res = INIT_RES_VALUE_INT;
    for (size_t i = 0; i < 4; i++) {
        assert(vector_get(vec, i, &res) == VEC_OK);
        assert(res == test_values[i]);
    }

    /* try to read from outside of the range */
    assert(vector_get(vec, 4, &res) == VEC_ERR_RANGE);

    /* check vector size */
    size_t size = INIT_SIZE_T;
    status = vector_size(vec, &size);
    assert(status == VEC_OK);
    assert(size == 4);

    /* free the vector*/
    vector_destroy(vec);
}

/**
  * @brief  Test push limits to vector.
  */
static void test_vector_push_limits(void)
{
    vector_t *vec = vector_create();
    assert(NULL != vec);

    /* fill the vector with limits values */
    vector_status_t status = VEC_ERR_NOMEM;

    status = vector_push_back(vec, 0);
    assert(status == VEC_OK);

    status = vector_push_back(vec, -1);
    assert(status == VEC_OK);

    status = vector_push_back(vec, INT_MAX);
    assert(status == VEC_OK);

    status = vector_push_back(vec, INT_MIN);
    assert(status == VEC_OK);

    /* read all values */
    int res = INIT_RES_VALUE_INT;
    status = vector_get(vec, 0, &res);
    assert(status == VEC_OK);
    assert(res == 0);

    status = vector_get(vec, 1, &res);
    assert(status == VEC_OK);
    assert(res == -1);

    status = vector_get(vec, 2, &res);
    assert(status == VEC_OK);
    assert(res == INT_MAX);

    status = vector_get(vec, 3, &res);
    assert(status == VEC_OK);
    assert(res == INT_MIN);

    /* check vector size */
    size_t size = INIT_SIZE_T;
    status = vector_size(vec, &size);
    assert(status == VEC_OK);
    assert(size == 4);

    /* free the vector */
    vector_destroy(vec);
}

/* Get tests -----------------------------------------------------------------*/
/**
  * @brief  Test get element via null vector or to null pointer. 
  */
static void test_null_vector_get(void)
{
    int res = INIT_RES_VALUE_INT;
    vector_status_t status = VEC_ERR_NOMEM;

    /* try to read a null pointer */
    status = vector_get(NULL, 2, &res);
    assert(status == VEC_ERR_NULL);
    assert(res == INIT_RES_VALUE_INT);

    /* create vector */
    vector_t *vec = vector_create();
    assert(NULL != vec);

    /* try to use NULL pointer as an output */
    status = vector_get(vec, 0, NULL);
    assert(status == VEC_ERR_NULL);
    assert(NULL != vec);

    /* push an element to the vector */
    int test_value = 89;
    status = vector_push_back(vec, test_value);
    assert(status == VEC_OK);

    /* try to get pushed element */
    status = vector_get(vec, 0, &res);
    assert(status == VEC_OK);
    assert(res == test_value);

    /* check vector size */
    size_t size = INIT_SIZE_T;
    status = vector_size(vec, &size);
    assert(status == VEC_OK);
    assert(size == 1);

    /* free the vector */
    vector_destroy(vec);
}

/**
  * @brief  Test get one element.  
  */
static void test_vector_get_one_element(void)
{
    vector_t *vec = vector_create();
    assert(NULL != vec);
    
    /* push one element */
    int test_value = 23;
    vector_status_t status = VEC_ERR_NOMEM;

    status = vector_push_back(vec, test_value);
    assert(status == VEC_OK);

    /* try to get pushed element */
    int res = INIT_RES_VALUE_INT;
    status = vector_get(vec, 0, &res);
    assert(status == VEC_OK);
    assert(res == test_value);

    /* try to get from the next index */
    status = vector_get(vec, 1, &res);
    assert(status == VEC_ERR_RANGE);

    /* check vector size */
    size_t size = INIT_SIZE_T;
    status = vector_size(vec, &size);
    assert(status == VEC_OK);
    assert(size == 1);
    
    /* free the vector */
    vector_destroy(vec);
}

/**
  * @brief  Test get from empty vector.
  */
static void test_vector_get_from_empty(void)
{
    vector_t *vec = vector_create();
    assert(NULL != vec);
    
    /* check vector size */
    vector_status_t status = VEC_ERR_NOMEM;
    size_t size = INIT_SIZE_T;
    status = vector_size(vec, &size);
    assert(status == VEC_OK);
    assert(size == 0);

    /* try to read from empty vector */
    int res = INIT_RES_VALUE_INT;
    for (size_t i = 0; i < 4; i++) {
        status = vector_get(vec, i, &res);
        assert(status == VEC_ERR_RANGE);
        assert(res == INIT_RES_VALUE_INT);
    }

    /* check vector size again */
    status = vector_size(vec, &size);
    assert(status == VEC_OK);
    assert(size == 0);

    /* free the vector */
    vector_destroy(vec);
}

/**
  * @brief  Test get after a push error.
*/
static void test_vector_get_after_push_err(void)
{
    vector_t *vec = vector_create();
    assert(NULL != vec);

    vector_status_t status = VEC_ERR_NOMEM;
    int values[4] = {21, 34, 56, 27};

    /* fill the vector */
    for (size_t i = 0; i < 4; i++) {
        status = vector_push_back(vec, values[i]);
        assert(status == VEC_OK);
    }

    /* vector is already full */
    status = vector_push_back(vec, 456);
    assert(status == VEC_ERR_NOMEM);

    /* get after error */
    int res = INIT_RES_VALUE_INT;
    for (size_t i = 0; i < 4; i++) {
        status = vector_get(vec, i, &res);
        assert(status == VEC_OK);
        assert(res == values[i]);
    }

    /* try to read outside of range */
    status = vector_get(vec, 4, &res);
    assert(status == VEC_ERR_RANGE);
    assert(res == values[3]);

    /* free the vector */
    vector_destroy(vec);
}

int main(void)
{
    /* Vector create/destroy tests */
    test_many_create_destroy();
    test_vector_null_destroy();

    /* Vector get size tests */
    test_null_vector_size();
    test_vector_size_from_empty();
    test_vector_size_after_pushes();

    /* Vector push tests */
    test_null_vector_push();
    test_vector_push_one_element();
    test_vector_push_until_full();
    test_vector_push_limits();
    
    /* Vector get tests */
    test_null_vector_get();
    test_vector_get_one_element();
    test_vector_get_from_empty();
    test_vector_get_after_push_err();

    return 0;
}