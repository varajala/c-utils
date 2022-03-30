#ifndef INTEGER_TYPES_H
#define INTEGER_TYPES_H

// This librabry is targeted for 64-bit platforms.
// If you wish to port this library for other targets,
// change the defenitions of the following integer types.

#define uint8 unsigned char
#define uint16 unsigned short
#define uint32 unsigned int
#define uint64 unsigned long int

#define int8 char
#define int16 short
#define int32 int
#define int64 long int

#define PLATFORM_POINTER_LENGTH 8

#ifndef NULL
#define NULL ((void*)0x00)
#endif

#endif
