BIN_DIRECTORY = ./bin
LIB_DIRECTORY = ./lib
INCLUDE_DIRECTORY = ./include

CC = gcc

build:
	$(CC) -c datastructures/array.c -o $(BIN_DIRECTORY)/array.obj -I $(INCLUDE_DIRECTORY)


clean:
	rm $(BIN_DIRECTORY)/*
	rm $(LIB_DIRECTORY)/*
