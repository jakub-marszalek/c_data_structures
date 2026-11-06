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
#include <stdint.h>
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
#define TEST_VEC_SIZE       (7)
#define INSERT_TEST_VALUE   (999)

/* Helpers -------------------------------------------------------------------*/
static size_t expected_capacity(size_t size)
{
    size_t cap = INIT_VEC_CAPACITY;
    while (cap < size) {
        cap *= CAP_MULTIPLIER;
    }
    return cap;
}

static void assert_vector_equals(const vector_t *vec, const int *expected, const size_t n)
{
    assert(NULL != vec);
    assert(n == 0 || NULL != expected);
    
    size_t size = INIT_SIZE_T;
    vector_status_t status = vector_size(vec, &size);
    assert(status == VEC_OK);
    assert(size == n);

    for (size_t i = 0; i < n; i++) {
        int value = INIT_RES_VALUE_INT;

        status = vector_get(vec, i, &value);
        assert(status == VEC_OK);
        assert(value == expected[i]);
    }
}

static void assert_size_equals(const vector_t *vec, const size_t expected)
{
    assert(NULL != vec);

    size_t size = INIT_SIZE_T;
    vector_status_t status = vector_size(vec, &size);
    assert(status == VEC_OK);
    assert(size == expected);
}

static void run_insert_no_realloc_test(
    const int *initial,
    const size_t initial_size,
    const size_t index,
    const int *expected,
    const size_t expected_size
) {
    assert(initial_size == 0 || NULL != initial);
    assert(NULL != expected);
    assert(expected_size == initial_size + 1);

    vector_t *vec = vector_create();
    assert(NULL != vec);

    vector_status_t status = VEC_ERR_NOMEM;
    for (size_t i = 0; i < initial_size; i++) {
        status = vector_push_back(vec, initial[i]);
        assert(status == VEC_OK);
    }
    assert_size_equals(vec, initial_size);

    size_t capacity_before = INIT_SIZE_T;
    status = vector_capacity(vec, &capacity_before);
    assert(status == VEC_OK);
    assert(capacity_before > initial_size);

    /* insert new value at given index */
    status = vector_insert(vec, index, INSERT_TEST_VALUE);
    assert(status == VEC_OK);
    assert_vector_equals(vec, expected, expected_size);
    
    size_t capacity_after = INIT_SIZE_T;
    status = vector_capacity(vec, &capacity_after);
    assert(status == VEC_OK);
    assert(capacity_before == capacity_after);

    vector_destroy(vec);
    vec = NULL;
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

/* Pop tests -----------------------------------------------------------------*/
/**
  * @brief  Test pop back functions with NULL pointers.
  */
static void test_null_vector_pop(void)
{
    vector_status_t status = VEC_ERR_NOMEM;
    int value              = INIT_RES_VALUE_INT;

    status = vector_pop_back(NULL, &value);
    assert(status == VEC_ERR_NULL);
    assert(value == INIT_RES_VALUE_INT);

    vector_t *vec = vector_create();
    assert(NULL != vec);

    int first_value   = 364344;
    size_t size       = INIT_SIZE_T;
    size_t capacity   = INIT_SIZE_T;

    status = vector_push_back(vec, first_value);
    assert(status == VEC_OK);

    status = vector_size(vec, &size);
    assert(status == VEC_OK);
    assert(size == 1);

    status = vector_capacity(vec, &capacity);
    assert(status == VEC_OK);
    assert(capacity == INIT_VEC_CAPACITY);

    /* NULL output vector given */
    status = vector_pop_back(vec, NULL);
    assert(status == VEC_ERR_NULL);

    /* element inside the vector is untouched */
    value = INIT_RES_VALUE_INT;
    status = vector_get(vec, 0, &value);
    assert(status == VEC_OK);
    assert(value == first_value);

    size = INIT_SIZE_T;
    status = vector_size(vec, &size);
    assert(status == VEC_OK);
    assert(size == 1);

    capacity = INIT_SIZE_T;
    status = vector_capacity(vec, &capacity);
    assert(status == VEC_OK);
    assert(capacity == INIT_VEC_CAPACITY);

    /* check push/get/pop after null errors */
    int second_value = 4325532;
    size             = INIT_SIZE_T;

    /* push element */
    status = vector_push_back(vec, second_value);
    assert(status == VEC_OK);

    size = INIT_SIZE_T;
    status = vector_size(vec, &size);
    assert(status == VEC_OK);
    assert(size == 2);

    /* check pushed element */
    value = INIT_RES_VALUE_INT;
    status = vector_get(vec, 1, &value);
    assert(status == VEC_OK);
    assert(value == second_value);

    /* pop element from vector  */
    value = INIT_RES_VALUE_INT;
    status = vector_pop_back(vec, &value);
    assert(status == VEC_OK);
    assert(value == second_value);
    
    size = INIT_SIZE_T;
    status = vector_size(vec, &size);
    assert(status == VEC_OK);
    assert(size == 1);

    value = INIT_RES_VALUE_INT;
    status = vector_get(vec, 1, &value);
    assert(status == VEC_ERR_RANGE);
    assert(value == INIT_RES_VALUE_INT);

    value = INIT_RES_VALUE_INT;
    status = vector_get(vec, 0, &value);
    assert(status == VEC_OK);
    assert(value == first_value);

    vector_destroy(vec);
    vec = NULL;
}

/**
  * @brief  Test pop one element from vector.
  */
static void test_vector_pop_one_element(void)
{
    vector_t *vec = vector_create();
    assert(NULL != vec);

    vector_status_t status = VEC_ERR_NOMEM;
    size_t size            = INIT_SIZE_T;
    size_t capacity        = INIT_SIZE_T;
    int last_value         = INIT_RES_VALUE_INT;

    for (size_t i = 0; i < TEST_VEC_SIZE; i++) {
        status = vector_push_back(vec, (int)i);
        assert(status == VEC_OK);

        last_value = (int)i;
    }

    status = vector_size(vec, &size);
    assert(status == VEC_OK);
    assert(size == TEST_VEC_SIZE);
    
    size_t expected_cap = expected_capacity(TEST_VEC_SIZE);
    status = vector_capacity(vec, &capacity);
    assert(status == VEC_OK);
    assert(capacity == expected_cap);

    /* pop one element */
    int popped_value = INIT_RES_VALUE_INT;

    status = vector_pop_back(vec, &popped_value);
    assert(status == VEC_OK);
    assert(popped_value == last_value);

    /* check size after pop */
    status = vector_size(vec, &size);
    assert(status == VEC_OK);
    assert(size == TEST_VEC_SIZE - 1);

    /* check capacity - should not change after pop */
    status = vector_capacity(vec, &capacity);
    assert(status == VEC_OK);
    assert(capacity == expected_cap);

    /* the popped element is gone; its index is out of range */
    int value = INIT_RES_VALUE_INT;
    
    status = vector_get(vec, TEST_VEC_SIZE - 1, &value);
    assert(status == VEC_ERR_RANGE);
    assert(value == INIT_RES_VALUE_INT);

    /* element right before it is untouched */
    status = vector_get(vec, TEST_VEC_SIZE - 2, &value);
    assert(status == VEC_OK);
    assert(value == last_value - 1);

    vector_destroy(vec);
    vec = NULL;
}

/**
  * @brief  Test pop from empty vector.
  */
static void test_vector_pop_from_empty(void)
{
    vector_t *vec = vector_create();
    assert(NULL != vec);

    vector_status_t status = VEC_ERR_NOMEM;
    size_t size            = INIT_SIZE_T;
    size_t capacity        = INIT_SIZE_T;
    int popped_value       = INIT_RES_VALUE_INT;

    for (size_t i = 0; i < MANY_TRIES_SIZE_T; i++) {
        status = vector_pop_back(vec, &popped_value);
        assert(status == VEC_ERR_EMPTY);
        assert(popped_value == INIT_RES_VALUE_INT);

        status = vector_size(vec, &size);
        assert(status == VEC_OK);
        assert(size == INIT_VEC_SIZE);

        status = vector_capacity(vec, &capacity);
        assert(status == VEC_OK);
        assert(capacity == INIT_VEC_CAPACITY);
    }

    int test_value = 43243;
    status = vector_push_back(vec, test_value);
    assert(status == VEC_OK);

    status = vector_size(vec, &size);
    assert(status == VEC_OK);
    assert(size == 1);

    status = vector_capacity(vec, &capacity);
    assert(status == VEC_OK);
    assert(capacity == INIT_VEC_CAPACITY);

    status = vector_pop_back(vec, &popped_value);
    assert(status == VEC_OK);
    assert(popped_value == test_value);

    status = vector_size(vec, &size);
    assert(status == VEC_OK);
    assert(size == 0);

    status = vector_capacity(vec, &capacity);
    assert(status == VEC_OK);
    assert(capacity == INIT_VEC_CAPACITY);

    vector_destroy(vec);
    vec = NULL;
}

/**
  * @brief  Test push and pop many elements.
  */
static void test_vector_many_pop_back(void)
{
    vector_t *vec = vector_create();
    assert(NULL != vec);

    vector_status_t status = VEC_ERR_NOMEM;
    size_t size            = INIT_SIZE_T;

    for (size_t i = 0; i < MANY_PUSHES_SIZE_T; i++) {
        status = vector_push_back(vec, (int)i);
        assert(status == VEC_OK);
    }

    status = vector_size(vec, &size);
    assert(status == VEC_OK);
    assert(size == MANY_PUSHES_SIZE_T);

    int value = INIT_RES_VALUE_INT;
    for (size_t i = MANY_PUSHES_SIZE_T; i > 0; i--) {
        status = vector_pop_back(vec, &value);
        assert(status == VEC_OK);
        assert(value == (int)(i - 1));
    }

    status = vector_size(vec, &size);
    assert(status == VEC_OK);
    assert(size == 0);

    /* try to get elements from empty vector */
    for (size_t i = 0; i < MANY_PUSHES_SIZE_T; i++) {
        value = INIT_RES_VALUE_INT;
        status = vector_get(vec, i, &value);
        assert(status == VEC_ERR_RANGE);
        assert(value == INIT_RES_VALUE_INT);
    }

    /* pop does not release allocated memory */
    status = VEC_ERR_NOMEM;
    size_t capacity = INIT_SIZE_T;
    
    status = vector_capacity(vec, &capacity);
    assert(status == VEC_OK);
    assert(capacity == expected_capacity(MANY_PUSHES_SIZE_T));

    vector_destroy(vec);
    vec = NULL;
}

/**
  * @brief  Test vector reuse after emptying.
  */
static void test_vector_use_after_empty(void)
{
    vector_t *vec = vector_create();
    assert(NULL != vec);

    int test_values[4]      = {46, 7546, 23412, 9850};
    size_t test_values_size = (sizeof(test_values) / sizeof(test_values[0]));
    vector_status_t status  = VEC_ERR_NOMEM;
    
    for (size_t i = 0; i < test_values_size; i++) {
        status = vector_push_back(vec, test_values[i]);
        assert(status == VEC_OK);
    }

    size_t size     = INIT_SIZE_T;
    size_t capacity = INIT_SIZE_T;

    status = vector_size(vec, &size);
    assert(status == VEC_OK);
    assert(size == test_values_size);

    status = vector_capacity(vec, &capacity);
    assert(status == VEC_OK);
    assert(capacity == expected_capacity(test_values_size));

    /* check if vector behaves as a LIFO queue */
    int value = INIT_RES_VALUE_INT;

    for (size_t i = test_values_size; i > 0; i--) {
        status = vector_pop_back(vec, &value);
        assert(status == VEC_OK);
        assert(value == test_values[i - 1]);
    
        status = vector_size(vec, &size);
        assert(status == VEC_OK);
        assert(size == (i - 1));

        status = vector_capacity(vec, &capacity);
        assert(status == VEC_OK);
        assert(capacity == expected_capacity(test_values_size));

        status   = VEC_ERR_NOMEM;
        size     = INIT_SIZE_T;
        capacity = INIT_SIZE_T;
        value    = INIT_RES_VALUE_INT;
    }

    status = vector_size(vec, &size);
    assert(status == VEC_OK);
    assert(size == 0);

    status = vector_capacity(vec, &capacity);
    assert(status == VEC_OK);
    assert(capacity == expected_capacity(test_values_size));

    value = INIT_RES_VALUE_INT;
    status = vector_get(vec, 0, &value);
    assert(status == VEC_ERR_RANGE);
    assert(value == INIT_RES_VALUE_INT);

    /* reuse the vector */
    int new_values[3]      = {1234, 9000, 2};
    size_t new_values_size = (sizeof(new_values) / sizeof(new_values[0]));
    
    for (size_t i = 0; i < new_values_size; i++) {
        status = vector_push_back(vec, new_values[i]);
        assert(status == VEC_OK);
    }

    status = vector_size(vec, &size);
    assert(status == VEC_OK);
    assert(size == new_values_size);

    status = vector_capacity(vec, &capacity);
    assert(status == VEC_OK);
    assert(capacity == expected_capacity(new_values_size));

    status = VEC_ERR_NOMEM;
    value  = INIT_RES_VALUE_INT;
    for (size_t i = 0; i < new_values_size; i++) {
        status = vector_get(vec, i, &value);
        assert(status == VEC_OK);
        assert(value == new_values[i]);

        status = VEC_ERR_NOMEM;
        value  = INIT_RES_VALUE_INT;
    }

    status = vector_get(vec, new_values_size, &value);
    assert(status == VEC_ERR_RANGE);
    assert(value == INIT_RES_VALUE_INT);

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

/* Set tests -----------------------------------------------------------------*/
/**
  * @brief  Test set element in NULL vector. 
  */
static void test_null_vector_set(void)
{
    vector_t *vec = vector_create();
    assert(NULL != vec);

    vector_status_t status = VEC_ERR_NOMEM;
    size_t size            = INIT_SIZE_T;
    size_t capacity        = INIT_SIZE_T;
    int test_value         = 57;
    int value              = INIT_RES_VALUE_INT;

    status = vector_push_back(vec, test_value);
    assert(status == VEC_OK);

    /* check vector properties before causing set error */
    status = vector_size(vec, &size);
    assert(status == VEC_OK);
    assert(size == 1);

    status = vector_capacity(vec, &capacity);
    assert(status == VEC_OK);
    assert(capacity == INIT_VEC_CAPACITY);

    status = vector_get(vec, 0, &value);
    assert(status == VEC_OK);
    assert(value == test_value);

    status   = VEC_ERR_NOMEM;
    size     = INIT_SIZE_T;
    capacity = INIT_SIZE_T;
    value    = INIT_RES_VALUE_INT;

    /* vector set failure on NULL pointer */
    int new_value = 85479;
    
    status = vector_set(NULL, 0, new_value);
    assert(status == VEC_ERR_NULL);

    /* vector should remain untouched */
    status = vector_size(vec, &size);
    assert(status == VEC_OK);
    assert(size == 1);

    status = vector_capacity(vec, &capacity);
    assert(status == VEC_OK);
    assert(capacity == INIT_VEC_CAPACITY);

    value = INIT_RES_VALUE_INT;
    status = vector_get(vec, 0, &value);
    assert(status == VEC_OK);
    assert(value == test_value);

    value = INIT_RES_VALUE_INT;
    status = vector_get(vec, 1, &value);
    assert(status == VEC_ERR_RANGE);
    assert(value == INIT_RES_VALUE_INT);

    vector_destroy(vec);
    vec = NULL;
}

/**
  * @brief  Test set element in empty vector.
  */
static void test_empty_vector_set(void)
{
    vector_t *vec = vector_create();
    assert(NULL != vec);

    vector_status_t status = VEC_ERR_NOMEM;
    int new_value          = 52314;

    for (size_t i = 0; i < MANY_TRIES_SIZE_T; i++) {
        status = vector_set(vec, i, new_value);
        assert(status == VEC_ERR_RANGE);

        status = VEC_ERR_NOMEM;
    }

    size_t size = INIT_SIZE_T;
    status = vector_size(vec, &size);
    assert(status == VEC_OK);
    assert(size == 0);

    size_t capacity = INIT_SIZE_T;
    status = VEC_ERR_NOMEM;
    status = vector_capacity(vec, &capacity);
    assert(status == VEC_OK);
    assert(capacity == INIT_VEC_CAPACITY);

    vector_destroy(vec);
    vec = NULL;
}

/**
  * @brief  Test set edge elements.
  */
static void test_vector_set_edge_elements(void)
{
    vector_t *vec = vector_create();
    assert(NULL != vec);

    int test_values[6]     = {4, 213, 53, 697, 235, 99992};
    size_t test_val_size   = (sizeof(test_values) / sizeof(test_values[0]));
    vector_status_t status = VEC_ERR_NOMEM;
    size_t size            = INIT_SIZE_T;
    size_t capacity        = INIT_SIZE_T;
    int value              = INIT_RES_VALUE_INT;

    for (size_t i = 0; i < test_val_size; i++) {
        status = vector_push_back(vec, test_values[i]);
        assert(status == VEC_OK);

        status = VEC_ERR_NOMEM;
    }

    for (size_t i = 0; i < test_val_size; i++) {
        status = vector_get(vec, i, &value);
        assert(status == VEC_OK);
        assert(value == test_values[i]);

        status = VEC_ERR_NOMEM;
        value  = INIT_RES_VALUE_INT;
    }

    status = vector_size(vec, &size);
    assert(status == VEC_OK);
    assert(size == test_val_size);

    status = vector_capacity(vec, &capacity);
    assert(status == VEC_OK);
    assert(capacity == expected_capacity(test_val_size));

    /* update test values array - values should match after sets */
    size_t start_index       = 0;
    int new_start_val        = 111111;
    test_values[start_index] = new_start_val;
    
    status = VEC_ERR_NOMEM;
    status = vector_set(vec, start_index, new_start_val);
    assert(status == VEC_OK);

    size_t mid_index         = 2;
    int new_mid_val          = 222222;
    test_values[mid_index]   = new_mid_val;

    status = VEC_ERR_NOMEM;
    status = vector_set(vec, mid_index, new_mid_val);
    assert(status == VEC_OK);

    size_t end_index         = test_val_size - 1;
    int new_end_val          = 777777;
    test_values[end_index]   = new_end_val;

    status = VEC_ERR_NOMEM;
    status = vector_set(vec, end_index, new_end_val);
    assert(status == VEC_OK);
    
    /* try to set beyond the range */
    status = VEC_ERR_NOMEM;
    status = vector_set(vec, test_val_size, 88888);
    assert(status == VEC_ERR_RANGE);
    
    /* check if new values were set correctly */
    status = VEC_ERR_NOMEM;
    value  = INIT_RES_VALUE_INT;
    
    for (size_t i = 0; i < test_val_size; i++) {
        status = vector_get(vec, i, &value);
        assert(status == VEC_OK);
        assert(value == test_values[i]);

        status = VEC_ERR_NOMEM;
        value  = INIT_RES_VALUE_INT;
    }

    status = vector_size(vec, &size);
    assert(status == VEC_OK);
    assert(size == test_val_size);

    status = vector_capacity(vec, &capacity);
    assert(status == VEC_OK);
    assert(capacity == expected_capacity(test_val_size));

    vector_destroy(vec);
    vec = NULL;
}

/* Insert tests --------------------------------------------------------------*/
/**
  * @brief  Test vector insert on null pointer.
  */
static void test_null_vector_insert(void)
{
    vector_status_t status = VEC_ERR_NOMEM;
    status = vector_insert(NULL, 0, INSERT_TEST_VALUE);
    assert(status == VEC_ERR_NULL);
}

/**
  * @brief  Test vector insert on empty vector.
  */
static void test_vector_insert_into_empty(void)
{
    vector_t *vec = vector_create();
    assert(NULL != vec);
    
    vector_status_t status = vector_insert(vec, 1, INSERT_TEST_VALUE);
    assert(status == VEC_ERR_RANGE);
    assert_vector_equals(vec, NULL, 0);

    /* index == 0, works as an append (vector_push_back())*/
    const int expected[]       = {INSERT_TEST_VALUE};
    const size_t expected_size = sizeof(expected) / sizeof(expected[0]);

    status = vector_insert(vec, 0, INSERT_TEST_VALUE);
    assert(status == VEC_OK);
    assert_vector_equals(vec, expected, expected_size);

    size_t capacity = INIT_SIZE_T;
    status = vector_capacity(vec, &capacity);
    assert(status == VEC_OK);
    assert(capacity == INIT_VEC_CAPACITY);

    vector_destroy(vec);
    vec = NULL;
}

/**
  * @brief  Test vector insert outside of range.
  */
static void test_vector_insert_outside_of_range(void)
{
    vector_t *vec = vector_create();
    assert(NULL != vec);

    vector_status_t status = VEC_ERR_NOMEM;
    int vector_data[TEST_VEC_SIZE];
    
    for (size_t i = 0; i < TEST_VEC_SIZE; i++) {
        status = vector_push_back(vec, (int)i);
        assert(status == VEC_OK);
        vector_data[i] = (int)i; 
    }

    /* insert outside of range return error - vector remains untouched */
    status = vector_insert(vec, TEST_VEC_SIZE + 1, INSERT_TEST_VALUE);
    assert(status == VEC_ERR_RANGE);
    assert_vector_equals(vec, vector_data, TEST_VEC_SIZE);

    status = vector_insert(vec, SIZE_MAX, INSERT_TEST_VALUE);
    assert(status == VEC_ERR_RANGE);
    assert_vector_equals(vec, vector_data, TEST_VEC_SIZE);

    vector_destroy(vec);
    vec = NULL;
}

/**
  * @brief  Test vector insert at the first index.
  */
static void test_vector_insert_at_first(void)
{
    const int initial[]  =                    {2, 3, 4, 5, 6, 7, 8};
    const int expected[] = {INSERT_TEST_VALUE, 2, 3, 4, 5, 6, 7, 8};
    const size_t initial_size  = sizeof(initial) / sizeof(initial[0]);
    const size_t expected_size = sizeof(expected) / sizeof(expected[0]);

    run_insert_no_realloc_test(initial, initial_size, 0, expected, expected_size);
}

/**
  * @brief  Test vector insert at the middle index.
  */
static void test_vector_insert_at_middle(void)
{
    const int initial[]  = {4, 5, 6, 7, 8};
    const int expected[] = {4, 5, INSERT_TEST_VALUE, 6, 7, 8};
    const size_t initial_size  = sizeof(initial) / sizeof(initial[0]);
    const size_t expected_size = sizeof(expected) / sizeof(expected[0]);

    run_insert_no_realloc_test(initial, initial_size, 2, expected, expected_size);
}

/**
  * @brief  Test vector insert at the end index.
  */
static void test_vector_insert_at_end(void)
{
    const int initial[]  = {4, 5, 6, 7, 8};
    const int expected[] = {4, 5, 6, 7, 8, INSERT_TEST_VALUE};
    const size_t initial_size  = sizeof(initial) / sizeof(initial[0]);
    const size_t expected_size = sizeof(expected) / sizeof(expected[0]);

    run_insert_no_realloc_test(initial, initial_size, initial_size, expected, expected_size);
}

/**
  * @brief  Test vector insert on full vector - realloc needed.
  */
static void test_vector_insert_with_realloc(void)
{
    vector_t *vec = vector_create();
    assert(NULL != vec);

    size_t capacity_before = INIT_SIZE_T;
    vector_status_t status = vector_capacity(vec, &capacity_before);
    assert(status == VEC_OK);
    assert(capacity_before == INIT_VEC_CAPACITY);
 
    /* fill the vector up to its capacity */
    for (size_t i = 0; i < capacity_before; i++) {
        status = vector_push_back(vec, (int)i);
        assert(status == VEC_OK);
    }
    assert_size_equals(vec, capacity_before);

    /* vector is full - insert at the middle -> realloc needed */
    const size_t index = capacity_before / 2;
    status = vector_insert(vec, index, INSERT_TEST_VALUE);
    assert(status == VEC_OK);

    size_t size_after = capacity_before + 1;
    assert_size_equals(vec, size_after);

    for (size_t i = 0; i < size_after; i++) {
        int value    = INIT_RES_VALUE_INT;
        int expected = INIT_RES_VALUE_INT;

        if (i < index) {
            expected = (int)i;
        } else if (i == index) {
            expected = INSERT_TEST_VALUE;
        } else {
            expected = (int)(i - 1);
        }

        status = vector_get(vec, i, &value);
        assert(status == VEC_OK);
        assert(value == expected);
    }

    size_t capacity_after = INIT_SIZE_T;
    status = vector_capacity(vec, &capacity_after);
    assert(status == VEC_OK);
    assert(capacity_after == expected_capacity(size_after));

    vector_destroy(vec);
    vec = NULL;
}

/* Clear tests ---------------------------------------------------------------*/
/**
  * @brief  Test vector clear on null pointer.
  */
static void test_null_vector_clear(void)
{
    vector_status_t status = VEC_ERR_NOMEM;
    status = vector_clear(NULL);
    assert(status == VEC_ERR_NULL);
}

/**
  * @brief  Test vector clear on big vector.
  */
static void test_big_vector_clear(void)
{
    vector_t *vec = vector_create();
    assert(NULL != vec);

    /* push many elements */
    vector_status_t status = VEC_ERR_NOMEM;
    for (size_t i = 0; i < MANY_PUSHES_SIZE_T; i++) {
        status = vector_push_back(vec, (int)i);
        assert(status == VEC_OK);
        status = VEC_ERR_NOMEM;
    }

    /* check size and capacity */
    size_t size = INIT_SIZE_T;
    status = vector_size(vec, &size);
    assert(status == VEC_OK);
    assert(size == MANY_PUSHES_SIZE_T);

    status = VEC_ERR_NOMEM;
    size_t capacity = INIT_SIZE_T;
    status = vector_capacity(vec, &capacity);
    assert(status == VEC_OK);
    assert(capacity == expected_capacity(MANY_PUSHES_SIZE_T));

    /* clear the vector - expected size is 0, capacity untouched */
    status = VEC_ERR_NOMEM;
    status = vector_clear(vec);
    assert(status == VEC_OK);

    status = VEC_ERR_NOMEM;
    size = INIT_SIZE_T;
    status = vector_size(vec, &size);
    assert(status == VEC_OK);
    assert(size == 0);

    status = VEC_ERR_NOMEM;
    capacity = INIT_SIZE_T;
    status = vector_capacity(vec, &capacity);
    assert(status == VEC_OK);
    assert(capacity == expected_capacity(MANY_PUSHES_SIZE_T));

    /* try to clear empty vector */
    status = VEC_ERR_NOMEM;
    status = vector_clear(vec);
    assert(status == VEC_OK);

    status = VEC_ERR_NOMEM;
    size = INIT_SIZE_T;
    status = vector_size(vec, &size);
    assert(status == VEC_OK);
    assert(size == 0);

    status = VEC_ERR_NOMEM;
    capacity = INIT_SIZE_T;
    status = vector_capacity(vec, &capacity);
    assert(status == VEC_OK);
    assert(capacity == expected_capacity(MANY_PUSHES_SIZE_T));

    vector_destroy(vec);
    vec = NULL;
}

/**
  * @brief  Test independence of two vectors.
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

    /* Vector pop tests */
    test_null_vector_pop();
    test_vector_pop_one_element();
    test_vector_pop_from_empty();
    test_vector_many_pop_back();
    test_vector_use_after_empty();
    
    /* Vector get tests */
    test_null_vector_get();
    test_vector_get_one_element();
    test_vector_get_from_empty();

    /* Vector set tests */
    test_null_vector_set();
    test_empty_vector_set();
    test_vector_set_edge_elements();

    /* Vector insert tests */
    test_null_vector_insert();
    test_vector_insert_into_empty();
    test_vector_insert_outside_of_range();
    test_vector_insert_at_first();
    test_vector_insert_at_middle();
    test_vector_insert_at_end();
    test_vector_insert_with_realloc(); 

    /* Vector clear tests */
    test_null_vector_clear();
    test_big_vector_clear();

    /* Two vectors test */
    test_two_vectors();

    return 0;
}