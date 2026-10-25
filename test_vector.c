/*
 * Copyright (c) 2026 Jakub Marszałek
 *
 * SPDX-License-Identifier: MIT
 *
 * This software is provided "as is", without warranty of any kind.
 * See the LICENSE file for details.
 */

/* Private includes ----------------------------------------------------------*/
#include <stdio.h>
#include <assert.h>
#include <limits.h>
#include "vector.h"

/* Private defines -----------------------------------------------------------*/
#define CAP_MULTIPLIER      (2)
#define INIT_RES_VALUE_INT  (-999)
#define INIT_SIZE_T         (99999)
#define INIT_VEC_SIZE       (0)
#define INIT_VEC_CAPACITY   (4)
#define MANY_PUSHES_SIZE_T  (98909)
#define MANY_TRIES_SIZE_T   (100000)
#define VEC_A_ARR_SIZE      (3)
#define VEC_B_ARR_SIZE      (2)

/* Helpers -------------------------------------------------------------------*/
static size_t expected_capacity(size_t size)
{
    size_t cap = INIT_VEC_CAPACITY;
    while (cap < size) {
        cap *= CAP_MULTIPLIER;
    }
    return cap;
}

/* Create destroy tests ------------------------------------------------------*/
/**
  * @brief  Test vector create and destroy functions.
  */
static void test_many_create_destroy(void)
{
    for (size_t i = 0; i < MANY_TRIES_SIZE_T; i++) {
        vector_t *vec = vector_create();
        assert(NULL != vec);
        
        /* check vector size and capacity */
        vector_status_t status = VEC_ERR_NOMEM;
        size_t size     = INIT_SIZE_T;
        size_t capacity = INIT_SIZE_T;
    
        /* size should be 0 - empty vector */
        status = vector_size(vec, &size);
        assert(status == VEC_OK);
        assert(size == INIT_VEC_SIZE);
        
        /* capcacity should be INIT_VEC_CAPACITY */
        status = vector_capacity(vec, &capacity);
        assert(status == VEC_OK);
        assert(capacity == INIT_VEC_CAPACITY);

        /* free the vector */
        vector_destroy(vec);
        vec = NULL;
    }
}

/* Vector size/capacity ------------------------------------------------------*/
/**
  * @brief  Test vector get size/capacity from null and write to null address.
  */
static void test_null_vector_size_capacity(void)
{
    vector_status_t status = VEC_ERR_NOMEM;
    size_t size     = INIT_SIZE_T;
    size_t capacity = INIT_SIZE_T;

    /* try to read from null vector */
    status = vector_size(NULL, &size);
    assert(status == VEC_ERR_NULL);
    assert(size == INIT_SIZE_T);

    status = vector_capacity(NULL, &capacity);
    assert(status == VEC_ERR_NULL);
    assert(size == INIT_SIZE_T);

    /* create valid vector */
    vector_t *vec = vector_create();
    assert(NULL != vec);
    
    /* should return 0 - empty vector */
    status = vector_size(vec, &size);
    assert(status == VEC_OK);
    assert(size == 0);

    /* should return INIT_VEC_CAPACITY */
    status = vector_capacity(vec, &capacity);
    assert(status == VEC_OK);
    assert(capacity == INIT_VEC_CAPACITY);

    /* try to write to null address */
    size     = INIT_SIZE_T;
    capacity = INIT_SIZE_T;

    status = vector_size(vec, NULL);
    assert(status == VEC_ERR_NULL);
    assert(size == INIT_SIZE_T);

    status = vector_capacity(vec, NULL);
    assert(status == VEC_ERR_NULL);
    assert(capacity == INIT_SIZE_T);

    /* valid reads */
    status = vector_size(vec, &size);
    assert(status == VEC_OK);
    assert(size == 0);

    status = vector_capacity(vec, &capacity);
    assert(status == VEC_OK);
    assert(capacity == INIT_VEC_CAPACITY);
    
    /* free the vector */
    vector_destroy(vec);
    vec = NULL;
}

/**
  * @brief  Test vector size/capacity from empty vector.
  */
static void test_vector_size_capacity_from_empty(void)
{
    /* create valid vector */
    vector_t *vec = vector_create();
    assert(NULL != vec);

    vector_status_t status = VEC_ERR_NOMEM;
    size_t size     = INIT_SIZE_T;
    size_t capacity = INIT_SIZE_T;

    /* size should always return 0 / capacity should return initial capacity */
    for (size_t i = 0; i < MANY_TRIES_SIZE_T; i++) {
        status = vector_size(vec, &size);
        assert(status == VEC_OK);
        assert(size == 0);

        status = vector_capacity(vec, &capacity);
        assert(status == VEC_OK);
        assert(capacity == INIT_VEC_CAPACITY);
    }

    /* free the vector */
    vector_destroy(vec);
    vec = NULL;
}

/**
  * @brief  Test vector size/capacity and data after many pushes.
  */
static void test_vector_after_many_pushes(void)
{
    /* initialize vector */
    vector_t *vec = vector_create();
    assert(NULL != vec);
        
    /* check vector size */
    vector_status_t status = VEC_ERR_NOMEM;
    size_t size     = INIT_SIZE_T;
    size_t capacity = INIT_SIZE_T;
    
    status = vector_size(vec, &size);
    assert(status == VEC_OK);
    assert(size == INIT_VEC_SIZE);

    status = vector_capacity(vec, &capacity);
    assert(status == VEC_OK);
    assert(capacity == INIT_VEC_CAPACITY);

    status   = VEC_ERR_NOMEM; 
    size     = INIT_SIZE_T;
    capacity = INIT_SIZE_T;

    /* push many values: size grows by 1, capacity grows only when needed */
    for (size_t i = 0; i < MANY_PUSHES_SIZE_T; i++) {
        status = vector_push_back(vec, (int)i);
        assert(status == VEC_OK);

        status = vector_size(vec, &size);
        assert(status == VEC_OK);
        assert(size == i + 1);

        status = vector_capacity(vec, &capacity);
        assert(status == VEC_OK);
        assert(capacity == expected_capacity(i + 1));
    }
    status = VEC_ERR_NOMEM;

    /* all data must survive every reallocation */
    for (size_t i = 0; i < MANY_PUSHES_SIZE_T; i++) {
        int value = INIT_RES_VALUE_INT;
        status = vector_get(vec, i, &value);
        assert(status == VEC_OK);
        assert(value == (int)i);
    }
    status = VEC_ERR_NOMEM;

    /* try to get out of range - output should be untouched */
    int value = INIT_RES_VALUE_INT;
    status = vector_get(vec, MANY_PUSHES_SIZE_T, &value);
    assert(status == VEC_ERR_RANGE);
    assert(value == INIT_RES_VALUE_INT);

    /* free the vector */
    vector_destroy(vec);
    vec = NULL;
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
    /* create empty vector */
    vector_t *vec = vector_create();
    assert(NULL != vec);

    vector_status_t status = VEC_ERR_NOMEM;
    size_t size            = INIT_SIZE_T;
    size_t capacity        = INIT_SIZE_T;
    int test_value         = 56;

    /* check vector initial size */
    status = vector_size(vec, &size);
    assert(status == VEC_OK);
    assert(size == INIT_VEC_SIZE);

    status = vector_capacity(vec, &capacity);
    assert(status == VEC_OK);
    assert(capacity == INIT_VEC_CAPACITY);

    /* push one element */
    status = vector_push_back(vec, test_value);
    assert(status == VEC_OK);
    
    /* check if pushed element is correct */
    int value = INIT_RES_VALUE_INT;
    assert(vector_get(vec, 0, &value) == VEC_OK);
    assert(value == test_value);

    /* check vector size after one push */
    status = vector_size(vec, &size);
    assert(status == VEC_OK);
    assert(size == 1);

    /* capacity should not change in this scenario */
    status = vector_capacity(vec, &capacity);
    assert(status == VEC_OK);
    assert(capacity == INIT_VEC_CAPACITY);

    /* check indexes outside of range */
    status = vector_get(vec, 1, &value); 
    assert(status == VEC_ERR_RANGE);

    status = vector_get(vec, 2, &value); 
    assert(status == VEC_ERR_RANGE);

    /* free the vector */
    vector_destroy(vec);
    vec = NULL;
}

/**
  * @brief  Test exact moment of vector capacity growth: 5th element triggers realloc.
  */
static void test_vector_growth_boundary(void)
{
    vector_t *vec = vector_create();
    assert(NULL != vec);

    /* fill the vector with test values (initial capacity is 4) */
    int test_values[5]     = {46, 12, 9, -4, 67};
    vector_status_t status = VEC_ERR_NOMEM;
    size_t size            = INIT_SIZE_T;
    size_t capacity        = INIT_SIZE_T;
    
    for (int i = 0; i < INIT_VEC_CAPACITY; i++) {
        status = vector_push_back(vec, test_values[i]);
        assert(status == VEC_OK);
    }

    /* check capacity - should not be changed */
    status = vector_capacity(vec, &capacity);
    assert(status == VEC_OK);
    assert(capacity == INIT_VEC_CAPACITY);

    status = vector_size(vec, &size);
    assert(status == VEC_OK);
    assert(size == INIT_VEC_CAPACITY);
    assert(size == capacity);

    /* one more push triggers data block growth */
    status   = VEC_ERR_NOMEM;
    size     = INIT_SIZE_T;
    capacity = INIT_SIZE_T;

    status = vector_push_back(vec, test_values[4]);
    assert(status == VEC_OK);

    status = vector_capacity(vec, &capacity);
    assert(status == VEC_OK);
    assert(capacity == INIT_VEC_CAPACITY * CAP_MULTIPLIER);

    status = vector_size(vec, &size);
    assert(status == VEC_OK);
    assert(size == INIT_VEC_CAPACITY + 1);

    /* all data must survive reallocation */
    int value = INIT_RES_VALUE_INT;
    status    = VEC_ERR_NOMEM;

    for (size_t i = 0; i < INIT_VEC_CAPACITY + 1; i++) {
        status = vector_get(vec, i, &value);
        assert(status == VEC_OK);
        assert(value == test_values[i]);
    }

    /* first index out of range still should return an error */
    value  = INIT_RES_VALUE_INT;
    status = vector_get(vec, INIT_VEC_CAPACITY + 1, &value);
    assert(status == VEC_ERR_RANGE);
    assert(value == INIT_RES_VALUE_INT);

    /* free the vector*/
    vector_destroy(vec);
    vec = NULL;
}

/**
  * @brief  Test push limits to vector.
  */
static void test_vector_push_limits(void)
{
    vector_t *vec = vector_create();
    assert(NULL != vec);

    /* fill the vector with limits values */
    int test_values[INIT_VEC_CAPACITY] = {0, -1, INT_MAX, INT_MIN};
    vector_status_t status = VEC_ERR_NOMEM;

    for (size_t i = 0; i < INIT_VEC_CAPACITY; i++) {
        status = vector_push_back(vec, test_values[i]);
        assert(status == VEC_OK);
    }

    /* read all values */
    int value = INIT_RES_VALUE_INT;
    status    = VEC_ERR_NOMEM;
    
    for (size_t i = 0; i < INIT_VEC_CAPACITY; i++) {
        status = vector_get(vec, i, &value);
        assert(status == VEC_OK);
        assert(value == test_values[i]);
    }

    /* check vector size and capacity */
    status          = VEC_ERR_NOMEM;
    size_t size     = INIT_SIZE_T;
    size_t capacity = INIT_SIZE_T;

    status = vector_size(vec, &size);
    assert(status == VEC_OK);
    assert(size == INIT_VEC_CAPACITY);

    status = vector_capacity(vec, &capacity);
    assert(status == VEC_OK);
    assert(capacity == INIT_VEC_CAPACITY);
    assert(capacity == size);

    /* free the vector */
    vector_destroy(vec);
    vec = NULL;
}

/* Get tests -----------------------------------------------------------------*/
/**
  * @brief  Test get element via null vector or to null pointer. 
  */
static void test_null_vector_get(void)
{
    vector_status_t status = VEC_ERR_NOMEM;
    int value              = INIT_RES_VALUE_INT;

    /* try to read a null pointer */
    status = vector_get(NULL, 0, &value);
    assert(status == VEC_ERR_NULL);
    assert(value == INIT_RES_VALUE_INT);

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
    status = vector_get(vec, 0, &value);
    assert(status == VEC_OK);
    assert(value == test_value);

    /* check vector size and capacity */
    size_t size     = INIT_SIZE_T;
    size_t capacity = INIT_SIZE_T;

    status = vector_size(vec, &size);
    assert(status == VEC_OK);
    assert(size == 1);

    status = vector_capacity(vec, &capacity);
    assert(status == VEC_OK);
    assert(capacity == INIT_VEC_CAPACITY);

    /* free the vector */
    vector_destroy(vec);
    vec = NULL;
}

/**
  * @brief  Test get one element.  
  */
static void test_vector_get_one_element(void)
{
    vector_t *vec = vector_create();
    assert(NULL != vec);
    
    /* push one element */
    vector_status_t status = VEC_ERR_NOMEM;
    int test_value         = 23;

    status = vector_push_back(vec, test_value);
    assert(status == VEC_OK);

    /* try to get pushed element */
    int value = INIT_RES_VALUE_INT;
    status = vector_get(vec, 0, &value);
    assert(status == VEC_OK);
    assert(value == test_value);

    /* try to get from the next index */
    status = vector_get(vec, 1, &value);
    assert(status == VEC_ERR_RANGE);

    /* check vector size and capacity */
    size_t size     = INIT_SIZE_T;
    size_t capacity = INIT_SIZE_T;

    status = vector_size(vec, &size);
    assert(status == VEC_OK);
    assert(size == 1);

    status = vector_capacity(vec, &capacity);
    assert(status == VEC_OK);
    assert(capacity == INIT_VEC_CAPACITY);
    
    /* free the vector */
    vector_destroy(vec);
    vec = NULL;
}

/**
  * @brief  Test get from empty vector.
  */
static void test_vector_get_from_empty(void)
{
    vector_t *vec = vector_create();
    assert(NULL != vec);
    
    /* check vector size and capacity */
    vector_status_t status = VEC_ERR_NOMEM;
    size_t size            = INIT_SIZE_T;
    size_t capacity        = INIT_SIZE_T;

    status = vector_size(vec, &size);
    assert(status == VEC_OK);
    assert(size == INIT_VEC_SIZE);

    status = vector_capacity(vec, &capacity);
    assert(status == VEC_OK);
    assert(capacity == INIT_VEC_CAPACITY);

    /* try to read from empty vector */
    int value = INIT_RES_VALUE_INT;
    for (size_t i = 0; i < MANY_TRIES_SIZE_T; i++) {
        status = vector_get(vec, i, &value);
        assert(status == VEC_ERR_RANGE);
        assert(value == INIT_RES_VALUE_INT);
    }

    /* check vector size and capacity again */
    status = vector_size(vec, &size);
    assert(status == VEC_OK);
    assert(size == INIT_VEC_SIZE);

    status = vector_capacity(vec, &capacity);
    assert(status == VEC_OK);
    assert(capacity == INIT_VEC_CAPACITY);

    /* free the vector */
    vector_destroy(vec);
    vec = NULL;
}

/* TODO: test realloc fail */

/**
  * @brief Test independence of two vectors.
  */
static void test_two_vectors(void)
{
    vector_t *vec_a = vector_create();
    vector_t *vec_b = vector_create();

    /* check if vectors were created successfully */
    assert(NULL != vec_a && NULL != vec_b);
    assert(vec_a != vec_b);

    vector_status_t status_a = VEC_ERR_NOMEM;
    vector_status_t status_b = VEC_ERR_NOMEM;
    size_t size_a            = INIT_SIZE_T;
    size_t size_b            = INIT_SIZE_T;
    size_t capacity_a        = INIT_SIZE_T;
    size_t capacity_b        = INIT_SIZE_T;

    /* check if both vectors are empty */
    status_a = vector_size(vec_a, &size_a);
    status_b = vector_size(vec_b, &size_b);
    assert(status_a == VEC_OK && status_b == VEC_OK);
    assert(size_a == INIT_VEC_SIZE && size_b == INIT_VEC_SIZE);

    /* check if initial capacity is correct */
    status_a = vector_capacity(vec_a, &capacity_a);
    status_b = vector_capacity(vec_b, &capacity_b);
    assert(status_a == VEC_OK && status_b == VEC_OK);
    assert(capacity_a == INIT_VEC_CAPACITY && capacity_b == INIT_VEC_CAPACITY);

    /* fill vectors with data */
    int vec_a_data[VEC_A_ARR_SIZE] = {23, 46, 12};
    int vec_b_data[VEC_B_ARR_SIZE] = {-4, 29};

    for (size_t i = 0; i < VEC_A_ARR_SIZE; i++) {
        status_a = vector_push_back(vec_a, vec_a_data[i]);
        assert(status_a == VEC_OK);
    }
    status_a = vector_size(vec_a, &size_a);
    assert(status_a == VEC_OK);
    assert(size_a == VEC_A_ARR_SIZE);
    status_a = VEC_ERR_NOMEM;

    for (size_t i = 0; i < VEC_B_ARR_SIZE; i++) {
        status_b = vector_push_back(vec_b, vec_b_data[i]);
        assert(status_b == VEC_OK);
    }
    status_b = vector_size(vec_b, &size_b);
    assert(status_b == VEC_OK);
    assert(size_b == VEC_B_ARR_SIZE);
    status_b = VEC_ERR_NOMEM;

    /* adding data to B must not change size of A */
    status_a = vector_size(vec_a, &size_a);
    assert(status_a == VEC_OK);
    assert(size_a == VEC_A_ARR_SIZE);
    status_a = VEC_ERR_NOMEM;

    /* read data from vectors */
    int data_a = INIT_RES_VALUE_INT;
    int data_b = INIT_RES_VALUE_INT;

    for (size_t i = 0; i < VEC_A_ARR_SIZE; i++) {
        status_a = vector_get(vec_a, i, &data_a);
        assert(status_a == VEC_OK);
        assert(data_a == vec_a_data[i]);
    }
    status_a = VEC_ERR_NOMEM;
    data_a   = INIT_RES_VALUE_INT;

    for (size_t i = 0; i < VEC_B_ARR_SIZE; i++) {
        status_b = vector_get(vec_b, i, &data_b);
        assert(status_b == VEC_OK);
        assert(data_b == vec_b_data[i]);
    }
    status_b = VEC_ERR_NOMEM;
    data_b   = INIT_RES_VALUE_INT;

    /* check both vectors boundaries */
    /* vector A */
    for (size_t i = 0; i < VEC_A_ARR_SIZE; i++) {
        status_a = vector_get(vec_a, i, &data_a);
        assert(status_a == VEC_OK);
    }
    status_a = vector_get(vec_a, VEC_A_ARR_SIZE, &data_a);
    assert(status_a == VEC_ERR_RANGE);

    /* vector B */
    for (size_t i = 0; i < VEC_B_ARR_SIZE; i++) {
        status_b = vector_get(vec_b, i, &data_b);
        assert(status_b == VEC_OK);
    }
    status_b = vector_get(vec_b, VEC_A_ARR_SIZE, &data_b);
    assert(status_b == VEC_ERR_RANGE);

    status_b = vector_get(vec_b, VEC_A_ARR_SIZE + 1, &data_b);
    assert(status_b == VEC_ERR_RANGE);

    /* free A and try to use B */
    status_b       = VEC_ERR_NOMEM;
    data_b         = INIT_RES_VALUE_INT;
    int test_value = 888;
    
    vector_destroy(vec_a);  /* free vector A */
    vec_a = NULL;

    status_b = vector_push_back(vec_b, test_value);
    assert(status_b == VEC_OK);

    status_b = vector_get(vec_b, 2, &data_b);
    assert(status_b == VEC_OK);
    assert(data_b == test_value);

    status_b = vector_size(vec_b, &size_b);
    assert(status_b == VEC_OK);
    assert(size_b == 3);

    status_b = vector_capacity(vec_b, &capacity_b);
    assert(status_b == VEC_OK);
    assert(capacity_b == INIT_VEC_CAPACITY);

    /* free vector B */
    vector_destroy(vec_b);
    vec_b = NULL;
}

int main(void)
{
    /* Vector create/destroy tests */
    test_many_create_destroy();
    test_vector_null_destroy();

    /* Vector get size/capacity tests */
    test_null_vector_size_capacity();
    test_vector_size_capacity_from_empty();
    test_vector_after_many_pushes();

    /* Vector push tests */
    test_null_vector_push();
    test_vector_push_one_element();
    test_vector_growth_boundary();
    test_vector_push_limits();
    
    /* Vector get tests */
    test_null_vector_get();
    test_vector_get_one_element();
    test_vector_get_from_empty();

    /* Two vectors test */
    test_two_vectors();

    return 0;
}