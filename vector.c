#include "vector.h"


/* Code example of how to use those functions
 *
 *
 *
 *
 *
 * */


static void extend_vector(Vector vect) {
    const unsigned int to_add = vect->mem_size / 2;
    vect->mem_size += (to_add > 0 ? to_add : 1);
    vect->array = realloc(vect->array, vect->mem_size);
}

Vector new_vector(const unsigned int elem_size, const unsigned int initial_capacity) {
    internVect *new = malloc(sizeof(internVect));
    if (new == NULL) {
        fprintf(stderr, "Fatal: failed to allocate %zu bytes.\n", n);
        return NULL;
    }
    new->elem_size = elem_size;
    new->array_size = 0;
    new->mem_size = (initial_capacity > 0 ? initial_capacity : 1) * sizeof(elem_size);
    new->array = safe_malloc(new->mem_size);
    return new;
}

void drop_vector(Vector vector) {
    free(vector->array);
    free(vector);
}

void vector_push(Vector restrict vector, void *restrict new_elem) {
    if (vector->array_size * vector->elem_size >= vector->mem_size) {
        extend_vector(vector);
    }
    memcpy(vector->array + vector->array_size * vector->elem_size, new_elem, vector->elem_size);
    ++vector->array_size;
}

void *vector_pop(Vector vector) {
    if (vector->array_size > 0) {
        --vector->array_size;
        return vector->array + vector->array_size * vector->elem_size;
    } else {
        return NULL;
    }
}

void vector_insert(Vector vector, void * new_elem, const unsigned int index) {
    if (index >= vector->array_size || vector < 0) {
        fprintf(stderr, "Error : index array out of range");
        return;
    }
    if (vector->array_size * vector->elem_size >= vector->mem_size) {
        extend_vector(vector);
    }
    void* slot = vector->array + vector->elem_size * index;
    memmove(slot + vector->elem_size, (const void *) slot, (vector->array_size - index) * vector->elem_size);
    memcpy(vector->array + index * vector->elem_size, new_elem, vector->elem_size);
    ++vector->array_size;
}

void * vector_remove(Vector vector, const unsigned int index) {
    if (index >= vector->array_size || vector < 0) {
        fprintf(stderr, "Error : index array out of range");
        return NULL;
    }
    void* slot = vector->array + vector->elem_size * index;
    memcpy(vector->array + vector->array_size * vector->elem_size, slot, vector->elem_size);
    memmove(slot, (const void *) slot + vector->elem_size, (vector->array_size - index - 1) * vector->elem_size);
    --vector->array_size;
    return vector->array + (vector->array_size + 1) * vector->elem_size;
}

void *vector_get(Vector vector, const unsigned int index) {
    if (index >= vector->array_size || vector < 0) {
        fprintf(stderr, "Error : index array out of range");
        return NULL;
    }
    return vector->array + index * vector->elem_size;
}

unsigned int vector_get_size(Vector vector) {
    return vector->array_size;
}

void vector_trim(Vector vector) {
    const unsigned int new_size = vector->elem_size * vector->array_size;
    vector->array = realloc(vector->array, new_size);
}
