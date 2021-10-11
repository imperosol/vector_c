//
// Created by thgir on 11/10/2021.
//

#ifndef TP4_VECTOR_H
#define TP4_VECTOR_H

#include "nyanpasu_lib.h"

typedef struct {
    void *array;
    unsigned int array_size;
    unsigned int elem_size;
    unsigned int mem_size;
} internVect;

typedef internVect *Vector;

Vector new_vector(unsigned int elem_size, unsigned int initial_capacity);

void vector_push(Vector vector, void *new_elem);

void *vector_pop(Vector vector);

void drop_vector(Vector vector);

void *vector_get(Vector vector, unsigned int index);

void vector_insert(Vector vector, void *new_elem, unsigned int index);

void *vector_remove(Vector vector, unsigned int index);

unsigned int vector_get_size(Vector vector);


#endif //TP4_VECTOR_H
