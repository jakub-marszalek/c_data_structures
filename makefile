CC     = gcc
CFLAGS = -Wall -Wextra -Wpedantic -Wshadow -Wconversion -g -fsanitize=address,undefined -Iinclude

.PHONY: test clean

test: build/test_vector
	./build/test_vector

build/test_vector: tests/test_vector.c src/vector.c include/vector.h
	mkdir -p build
	$(CC) $(CFLAGS) tests/test_vector.c src/vector.c -o build/test_vector

clean:
	rm -rf build