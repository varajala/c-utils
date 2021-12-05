BIN_DIRECTORY = ./bin
LIB_DIRECTORY = ./lib
INCLUDE_DIRECTORY = ./include

CC = gcc

build:
	$(CC) -c datastructures/array.c -o $(BIN_DIRECTORY)/array.obj -I $(INCLUDE_DIRECTORY) -Wall


linux-tests:
	$(CC) -c datastructures/array.c -o $(BIN_DIRECTORY)/array.obj -I $(INCLUDE_DIRECTORY) -Wall -g

	$(CC) -c ./test-utils/linux/linux_allocator.c -o $(BIN_DIRECTORY)/linux_allocator.obj \
	-I $(INCLUDE_DIRECTORY) -I ./test-utils -Wall -g

	$(CC) -c ./tests/linux/array_tests.c -o $(BIN_DIRECTORY)/linux_array_test.obj \
	-I $(INCLUDE_DIRECTORY) -I ./test-utils -Wall -g

	$(CC) -o $(BIN_DIRECTORY)/linux-tests-exe \
	$(BIN_DIRECTORY)/array.obj \
	$(BIN_DIRECTORY)/linux_allocator.obj \
	$(BIN_DIRECTORY)/linux_array_test.obj


clean:
	rm $(BIN_DIRECTORY)/*
	rm $(LIB_DIRECTORY)/*
