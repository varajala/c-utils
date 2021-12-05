#ifndef ALLOCATOR_IMPLEMENTATION_H
#define ALLOCATOR_IMPLEMENTATION_H

#include "allocator.h"
#include "integer_types.h"

Allocator* allocator_create();

void allocator_free(Allocator *allocator);

#endif
