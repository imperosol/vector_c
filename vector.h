//
// Created by thgir on 11/10/2021.
//

#ifndef TP4_VECTOR_H
#define TP4_VECTOR_H

#include "nyanpasu_lib.h"

#define E_INCOMPATIBLE_TYPES 1

#define v_elem(type, elem_ptr) *((type *) elem_ptr)
#define v_size(vector) vector_get_size(vector)
#define v_get(vector, index) vector_get(vector, index)
#define v_set(vector, index, new_elem) vector_set(vector, index, new_elem)
#define v_type(type) sizeof(type)

// array_size * elem_size != mem_size
typedef struct {
    void *array;
    unsigned int array_size;
    unsigned int elem_size;
    unsigned int mem_size;
} internVect;
typedef internVect *Vector;

Vector new_vector(unsigned int elem_size, unsigned int initial_capacity);

Vector new_vector_from_array(const void* restrict array, unsigned int elem_size, unsigned int elem_count);

Vector new_vector_from_vector(Vector toCopy);

void vector_push(Vector vector, void *new_elem);

void *vector_pop(Vector vector);

void drop_vector(Vector vector);

void clear_vector(Vector vector);

void vector_trim(Vector vector);

void *vector_get(Vector vector, unsigned int index);

void vector_set(Vector vector, unsigned int index, const void * new_elem);

void vector_insert(Vector vector, void *new_elem, unsigned int index);

void *vector_remove(Vector vector, unsigned int index);

unsigned int vector_get_size(Vector vector);

void vector_push_array(Vector restrict vector, const void *restrict array, unsigned int array_size);

void vector_append(Vector restrict dest, Vector restrict source);

void vector_append_self(Vector restrict dest, Vector restrict source);
#endif //TP4_VECTOR_H
