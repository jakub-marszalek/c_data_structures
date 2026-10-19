CC     = gcc
CFLAGS = -Wall -Wextra -Wpedantic -Wshadow -Wconversion -g -fsanitize=address,undefined

.PHONY: test clean

test: test_vector
	./test_vector

test_vector: test_vector.c vector.c vector.h
	$(CC) $(CFLAGS) test_vector.c vector.c -o test_vector

clean:
	rm -f test_vector