//
// Created by thgir on 11/10/2021.
//

#ifndef TP4_VECTOR_H
#define TP4_VECTOR_H

#include "nyanpasu_lib.h"

#define E_INCOMPATIBLE_TYPES 1
#define E_OUT_OF_RANGE 2

#define v_elem(type, elem_ptr) *((type *) elem_ptr)
#define v_size(vector) vector->array_size
#define v_get(vector, index) vector_get(vector, index)
#define v_set(vector, index, new_elem) vector_set(vector, index, new_elem)
#define v_swap(vector, a, b) vector_swap(vector, a, b)
#define v_type(type) sizeof(type)

#define v_display(vector, type, format) for (int i = 0; i < vector->array_size; ++i)\
                                            printf(format, *((type *) vector->array + i))

typedef struct {
    void *array;
    unsigned int array_size;
    unsigned int elem_size;
    unsigned int mem_size;
} internVect;
typedef internVect *Vector;

__attribute__((unused)) Vector new_vector(unsigned int elem_size, unsigned int initial_capacity);

__attribute__((unused)) Vector
new_vector_from_array(const void *array, unsigned int elem_size, unsigned int elem_count);

__attribute__((unused)) Vector new_vector_from_vector(Vector toCopy);

__attribute__((unused)) void vector_push(Vector vector, void *new_elem);

__attribute__((unused)) void *vector_pop(Vector vector);

__attribute__((unused)) void drop_vector(Vector vector);

__attribute__((unused)) void clear_vector(Vector vector);

__attribute__((unused)) void vector_trim(Vector vector);

__attribute__((unused)) void *vector_get(Vector vector, unsigned int index);

__attribute__((unused)) void vector_set(Vector vector, unsigned int index, const void *new_elem);

__attribute__((unused)) void vector_insert(Vector vector, void *new_elem, unsigned int index);

__attribute__((unused)) void *vector_remove(Vector vector, unsigned int index);

__attribute__((unused)) void *vector_remove_val(Vector vector, const void *val);

__attribute__((unused)) unsigned int vector_get_size(Vector vector);

__attribute__((unused)) void
vector_push_array(Vector restrict vector, const void *restrict array, unsigned int array_size);

__attribute__((unused)) void vector_append(Vector restrict dest, Vector restrict source);

__attribute__((unused)) void vector_append_self(Vector restrict self);

__attribute__((unused)) void vector_swap(Vector restrict vect, unsigned int a, unsigned int b);
#endif //TP4_VECTOR_H
