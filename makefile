BIN_DIRECTORY = ./bin
LIB_DIRECTORY = ./lib
INCLUDE_DIRECTORY = ./include

CC = gcc

build:
	$(CC) -c datastructures/array.c -o $(BIN_DIRECTORY)/array.obj -I $(INCLUDE_DIRECTORY) -Wall -std=c99


linux-tests:
	$(CC) -c datastructures/array.c -o $(BIN_DIRECTORY)/array.obj -I $(INCLUDE_DIRECTORY) -Wall -g -std=c99

	$(CC) -c ./test-utils/linux/linux_allocator.c -o $(BIN_DIRECTORY)/linux_allocator.obj \
	-I $(INCLUDE_DIRECTORY) -I ./test-utils -Wall -g -std=c99

	$(CC) -c ./tests/linux/array_tests.c -o $(BIN_DIRECTORY)/linux_array_test.obj \
	-I $(INCLUDE_DIRECTORY) -I ./test-utils -Wall -g -std=c99

	$(CC) -o $(BIN_DIRECTORY)/linux-tests-exe \
	$(BIN_DIRECTORY)/array.obj \
	$(BIN_DIRECTORY)/linux_allocator.obj \
	$(BIN_DIRECTORY)/linux_array_test.obj


clean:
	rm $(BIN_DIRECTORY)/*
	rm $(LIB_DIRECTORY)/*
