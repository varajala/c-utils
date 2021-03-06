BIN_DIRECTORY = ./bin
LIB_DIRECTORY = ./lib

DEBUG_BUILD_FLAGS = -Wall -Wextra -Wno-unused-parameter -pedantic -std=c99 -g
RELEASE_BUILD_FLAGS = -Wall -Wextra -pedantic -std=c99 -O2

STANDALONE_FLAGS = -nostdlib -nodefaultlibs -nostdinc 

release:
	$(CC) $(RELEASE_BUILD_FLAGS) c-utils.c -o $(LIB_DIRECTORY)/libc-utils.so -shared -fPIC $(STANDALONE_FLAGS) 

test:
	$(CC) $(DEBUG_BUILD_FLAGS) -I . c-utils.c tests/main.c -o $(BIN_DIRECTORY)/test-exe

clean:
	rm $(BIN_DIRECTORY)/*
	rm $(LIB_DIRECTORY)/*
