BIN_DIRECTORY=./bin
LIB_DIRECTORY=./lib
INCLUDE_DIRECTORY=./include
CC=gcc

$CC -o $BIN_DIRECTORY/test-program-exe test-program.c -I $INCLUDE_DIRECTORY -L $LIB_DIRECTORY -lc-utils
