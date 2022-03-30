LIB_FILE = libc-utils.so
BUILD_FLAGS = -Wall -Wextra -pedantic -std=c99 -O2
STANDALONE_FLAGS = -nostdlib -nolibc -nodefaultlibs -nostdinc 

release:
	$(CC) $(BUILD_FLAGS) c-utils.c -o $(LIB_FILE) -shared -fPIC $(STANDALONE_FLAGS) 

clean:
	rm -f $(LIB_FILE)
