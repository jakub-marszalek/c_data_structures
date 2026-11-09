/*
 * Copyright (c) 2026 Jakub Marszałek
 *
 * SPDX-License-Identifier: MIT
 *
 * This software is provided "as is", without warranty of any kind.
 * See the LICENSE file for details.
 */

#include <stdio.h>
#include <stdlib.h>
#include "vector.h"

int main(void)
{
    /* Create a new vector */
    vector_t *vec = vector_create();
    if (NULL == vec) {
        fprintf(stderr, "Memory allocation for vector failed!\n");
        return EXIT_FAILURE;
    }

    if (vector_push_back(vec, 3) != VEC_OK) {
        fprintf(stderr, "error\n");
    }
    if (vector_push_back(vec, 1) != VEC_OK) {
        fprintf(stderr, "error\n");
    }
    if (vector_push_back(vec, 2) != VEC_OK) {
        fprintf(stderr, "error\n");
    }
    if (vector_push_back(vec, 4) != VEC_OK) {
        fprintf(stderr, "error\n");
    }
    if (vector_push_back(vec, 5) != VEC_OK) {
        fprintf(stderr, "error\n");
    }

    int x;
    for (size_t i = 0; i < 5; i++) {
        if (vector_get(vec, i, &x) != VEC_OK) {
            fprintf(stderr, "read error\n");
        } else {
            fprintf(stdout, "vec[%zu] = %d\n", i, x);
        }
    }

    /* Destroy the vector */
    vector_destroy(vec);
    vec = NULL;

    return 0;
}