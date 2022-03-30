BIN_DIRECTORY = ./bin
LIB_DIRECTORY = ./lib
INCLUDE_DIRECTORY = ./include

DEBUG_BUILD_FLAGS = -Wall -Wextra -Wno-unused-parameter -pedantic -std=c99 -g
RELEASE_BUILD_FLAGS = -Wall -Wextra -pedantic -std=c99 -O2

STANDALONE_FLAGS = -nostdlib -nolibc -nodefaultlibs -nostdinc 

release:
	$(CC) -I $(INCLUDE_DIRECTORY) $(RELEASE_BUILD_FLAGS) src/*.c -o $(LIB_DIRECTORY)/libc-utils.so -shared -fPIC $(STANDALONE_FLAGS) 

test:
	$(CC) -I $(INCLUDE_DIRECTORY) $(DEBUG_BUILD_FLAGS) src/*.c tests/main.c -o $(BIN_DIRECTORY)/test-exe

clean:
	rm $(BIN_DIRECTORY)/*
	rm $(LIB_DIRECTORY)/*
