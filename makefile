BIN_DIRECTORY = ./bin
LIB_DIRECTORY = ./lib
INCLUDE_DIRECTORY = ./include

CC = gcc

build:
	$(CC) -c src/array.c -o $(BIN_DIRECTORY)/array.obj -I $(INCLUDE_DIRECTORY) -Wall -std=c99
	$(CC) -c src/bump_allocator.c -o $(BIN_DIRECTORY)/bump_allocator.obj -I $(INCLUDE_DIRECTORY) -Wall -std=c99
	$(CC) -c src/arena_allocator.c -o $(BIN_DIRECTORY)/arena_allocator.obj -I $(INCLUDE_DIRECTORY) -Wall -std=c99


tests:
	$(CC) -c src/array.c -o $(BIN_DIRECTORY)/array.obj -I $(INCLUDE_DIRECTORY) -Wall -g -std=c99
	$(CC) -c src/bump_allocator.c -o $(BIN_DIRECTORY)/bump_allocator.obj -I $(INCLUDE_DIRECTORY) -Wall -std=c99
	$(CC) -c src/arena_allocator.c -o $(BIN_DIRECTORY)/arena_allocator.obj -I $(INCLUDE_DIRECTORY) -Wall -std=c99

	$(CC) -c tests/main.c -o $(BIN_DIRECTORY)/test_main.obj -I $(INCLUDE_DIRECTORY) -Wall -g -std=c99

	$(CC) -o $(BIN_DIRECTORY)/tests-exe \
	$(BIN_DIRECTORY)/array.obj \
	$(BIN_DIRECTORY)/bump_allocator.obj \
	$(BIN_DIRECTORY)/arena_allocator.obj \
	$(BIN_DIRECTORY)/test_main.obj


clean:
	rm $(BIN_DIRECTORY)/*
	rm $(LIB_DIRECTORY)/*
