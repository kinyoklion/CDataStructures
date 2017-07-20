//This file provides a "templated" vector implementation.
//The type of the vector is specified by defining a TYPE and then importing the
//header.
//For example:

// #define TYPE int
// #include "vector.h"
// #undef TYPE

//This can be done multiple times to create vector implementations for different
//types.

//Each different TYPE inclusion will generate a storage struct and associated
//implementation methods.

//For int the following struct and methods are generated:
// struct Vector_int;
// struct Vector_int* vec_Create_int(size_t chunk_size);
// struct Vector_int* vec_CreateDefault_int();
// int vec_Get_int(struct Vector_int* self, size_t index);
// void vec_Push_int(struct Vector_int* self, int value);
// int vec_Pop_int(struct Vector_int* self);
// void vec_Release_int(struct Vector_int* self);
// size_t vec_GetSize_int(struct Vector_int* self)

#include<stdlib.h>
#include<string.h>

//This section of macros should be the same for all type specific generations of
//the vector. It only needs to be, and only can be, defined once.
#ifndef VEC_MACROS_DEFINED
    #define TOKENCAT(x, y) x ## y
    
    #define VEC_STRUCT(T) TOKENCAT(Vector_, T)
    #define VEC_CREATE(T) TOKENCAT(vec_Create_, T)
    #define VEC_CREATE_DEFAULT(T) TOKENCAT(vec_CreateDefault_, T)
    #define VEC_GET(T) TOKENCAT(vec_Get_, T)
    #define VEC_PUSH(T) TOKENCAT(vec_Push_, T)
    #define VEC_POP(T) TOKENCAT(vec_Pop_, T)
    #define VEC_RELEASE(T) TOKENCAT(vec_Release_, T)
    #define VEC_GET_SIZE(T) TOKENCAT(vec_GetSize_, T)
    
    #define VEC_DEFAULT_SIZE 20
#endif

//Structure which represents a vector.
struct VEC_STRUCT(TYPE)
{
    size_t capacity;
    size_t size;
    size_t allocation_chunk;
    TYPE (*data)[];
};

//Create a vector with the requested chunk size. Each time the vector needs to
//increase capacity it will do so by the chunk size.
struct VEC_STRUCT(TYPE)* VEC_CREATE(TYPE)(size_t chunk_size)
{
    struct VEC_STRUCT(TYPE)* new_vector = malloc(sizeof(struct VEC_STRUCT(TYPE)));
    new_vector->allocation_chunk = chunk_size;
    new_vector->data = malloc(sizeof(TYPE) * new_vector->allocation_chunk);
    new_vector->capacity = new_vector->allocation_chunk;
    return new_vector;
};

//Create a vector with a default chunk size.
struct VEC_STRUCT(TYPE)* VEC_CREATE_DEFAULT(TYPE)()
{
    return VEC_CREATE(TYPE)(VEC_DEFAULT_SIZE);
}

//Get an item from the vector.
TYPE VEC_GET(TYPE)(struct VEC_STRUCT(TYPE)* self, size_t index)
{
    return (*self->data)[index];
}

//Push an item into the vector.
void VEC_PUSH(TYPE)(struct VEC_STRUCT(TYPE)* self, TYPE value)
{
    if(self->size >= self->capacity)
    {
        size_t new_capacity = self->capacity + self->allocation_chunk;
        TYPE (*new_data)[] = malloc(sizeof(TYPE) * new_capacity);
        memcpy(new_data, self->data, self->size * sizeof(TYPE));
        free(self->data);
        self->data = new_data;
        self->capacity = new_capacity;
    }
    (*self->data)[self->size] = value;
    self->size++;
}

//Pop an item from the vector. Returns the item which was poppsed. Popping an
//item removes it.
TYPE VEC_POP(TYPE)(struct VEC_STRUCT(TYPE)* self)
{
    self->size--;
    return (*self->data)[self->size];
}

//Release the vector. Does not release items that the vector may contain.
void VEC_RELEASE(TYPE)(struct VEC_STRUCT(TYPE)* self)
{
    free(self);
}

//Get the size of the vector.
size_t VEC_GET_SIZE(TYPE)(struct VEC_STRUCT(TYPE)* self)
{
    return self->size;
}