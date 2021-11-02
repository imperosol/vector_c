#include "vector.h"


__attribute__((unused)) static void extend_vector(Vector vect) {
    const unsigned int to_add = vect->mem_size / 2;
    vect->mem_size += (to_add > vect->elem_size ? to_add : vect->elem_size);
    vect->array = realloc(vect->array, vect->mem_size);
}

__attribute__((unused)) Vector new_vector(const unsigned int elem_size, const unsigned int initial_capacity) {
    internVect *new = malloc(sizeof(internVect));
    if (new == NULL) {
        fprintf(stderr, "Error: failed to allocate %zu bytes.\n", sizeof(internVect));
        return NULL;
    }
    new->elem_size = elem_size;
    new->array_size = 0;
    new->mem_size = (initial_capacity > 0 ? initial_capacity : 1) * elem_size;
    new->array = safe_malloc(new->mem_size);
    return new;
}

__attribute__((unused)) Vector
new_vector_from_array(const void *array, unsigned int elem_size, unsigned int elem_count) {
    internVect *new = malloc(sizeof(internVect));
    if (new == NULL) {
        fprintf(stderr, "Error: failed to allocate %zu bytes.\n", sizeof(internVect));
        return NULL;
    }
    new->elem_size = elem_size;
    new->array_size = elem_count;
    new->mem_size = elem_count * elem_size;
    new->array = malloc(new->mem_size);
    memcpy(new->array, array, new->mem_size);
    return new;
}

__attribute__((unused)) Vector new_vector_from_vector(Vector toCopy) {
    internVect *new = malloc(sizeof(internVect));
    if (new == NULL) {
        fprintf(stderr, "Error: failed to allocate %zu bytes.\n", sizeof(internVect));
        return NULL;
    }
    new->elem_size = toCopy->elem_size;
    new->array_size = toCopy->array_size;
    new->mem_size = toCopy->mem_size;
    new->array = malloc(new->mem_size);
    memcpy(new->array, toCopy->array, new->mem_size);
    return new;
}


__attribute__((unused)) void drop_vector(Vector vector) {
    free(vector->array);
    free(vector);
}

__attribute__((unused)) __attribute__((unused)) void clear_vector(Vector vector) {
    vector->array_size = 0;
    vector->mem_size = 0;
    free(vector->array);
    vector->array = NULL;
}

__attribute__((unused)) __attribute__((unused)) void vector_push(Vector restrict vector, void *restrict new_elem) {
    while ((vector->array_size + 1) * vector->elem_size > vector->mem_size) {
        extend_vector(vector);
    }
    memcpy(vector->array + vector->array_size * vector->elem_size, new_elem, vector->elem_size);
    ++vector->array_size;
}

__attribute__((unused)) __attribute__((unused)) void vector_push_array(Vector restrict vector, const void *restrict array, const unsigned int array_size) {
    while ((vector->array_size + array_size) * vector->elem_size > vector->mem_size) {
        extend_vector(vector);
    }
    memcpy(vector->array + vector->array_size * vector->elem_size, array, vector->elem_size * array_size);
    vector->array_size += array_size;
}

__attribute__((unused)) void *vector_pop(Vector vector) {
    if (vector->array_size > 0) {
        --vector->array_size;
        return vector->array + vector->array_size * vector->elem_size;
    } else {
        return NULL;
    }
}

__attribute__((unused)) void vector_insert(Vector vector, void * new_elem, const unsigned int index) {
    if (index >= vector->array_size) {
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

__attribute__((unused)) void * vector_remove(Vector vector, const unsigned int index) {
    if (index >= vector->array_size) {
        fprintf(stderr, "Error : index array out of range");
        return NULL;
    }
    void* slot = vector->array + vector->elem_size * index;
    memcpy(vector->array + vector->array_size * vector->elem_size, slot, vector->elem_size);
    memmove(slot, (const void *) slot + vector->elem_size, (vector->array_size - index - 1) * vector->elem_size);
    --vector->array_size;
    return vector->array + (vector->array_size + 1) * vector->elem_size;
}

__attribute__((unused)) void * vector_remove_val(Vector vector, const void* val) {
    for (int i = 0; i < vector->array_size; ++i) {
        if (memcmp(vector->array + i * vector->elem_size, val, vector->elem_size) == 0) {
            return vector_remove(vector, i);
        }
    }
    return NULL;
}

__attribute__((unused)) void *vector_get(Vector vector, const unsigned int index) {
    if (index >= vector->array_size) {
        fprintf(stderr, "Error : index array out of range");
        return NULL;
    }
    return vector->array + index * vector->elem_size;
}

__attribute__((unused)) void vector_set(Vector vector, const unsigned int index, const void * new_elem) {
    if (index >= vector->array_size) {
        fprintf(stderr, "Error : list index out of range");
        return;
    }
    memcpy(vector->array + index * vector->elem_size, new_elem, vector->elem_size);
}


__attribute__((unused)) unsigned int vector_get_size(Vector vector) {
    return vector->array_size;
}

__attribute__((unused)) void vector_trim(Vector vector) {
    vector->mem_size = vector->elem_size * vector->array_size;
    vector->array = realloc(vector->array, vector->mem_size);
}

__attribute__((unused)) void vector_append(Vector restrict dest, Vector restrict source) {
    if (dest->elem_size != source->elem_size) {
        fprintf(stderr, "Error : cannot concatenate vectors of different v_type");
        exit(E_INCOMPATIBLE_TYPES);
    }
    if (dest->elem_size * (dest->array_size + source->array_size) > dest->mem_size) {
        dest->mem_size = dest->elem_size * (dest->array_size + source->array_size);
        dest->array = realloc(dest->array, dest->mem_size);
    }
    memcpy(dest->array + dest->array_size * dest->elem_size,
           source->array,
           source->array_size * source->elem_size);
    dest->array_size += source->array_size;
}

__attribute__((unused)) void vector_append_self(Vector self) {
    if (self->elem_size * (self->array_size + self->array_size) > self->mem_size) {
        self->mem_size = self->elem_size * (self->array_size + self->array_size);
        self->array = realloc(self->array, self->mem_size);
    }
    memcpy(self->array + self->array_size * self->elem_size,
           self->array,
           self->array_size * self->elem_size);
    self->array_size += self->array_size;
}


__attribute__((unused)) void vector_swap(Vector restrict vect, const unsigned int a, const unsigned int b) {
    if (a >= vect->array_size || b >= vect->array_size) {
        fprintf(stderr, "Error : vector index out of range");
        return;
    }
    void *val_a = vect->array + a*vect->elem_size, *val_b = vect->array + b*vect->elem_size;
    void *tmp = malloc(vect->elem_size);
    if (tmp == NULL) {
        fprintf(stderr, "Couldn't perform swap : unavailable temporary memory for operation");
        return;
    }
    memcpy(tmp, val_a, vect->elem_size);
    memcpy(val_a, val_b, vect->elem_size);
    memcpy(val_b, tmp, vect->elem_size);
    free(tmp);
}

