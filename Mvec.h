//
// Created by victor on 07.05.23.
//

#ifndef SHIZX_MVEC_H
#define SHIZX_MVEC_H

#include <stdlib.h>
#include <string.h>

typedef struct {
    void *things;
    size_t sizeof_element;
    size_t len;
    size_t capacity;
} Mvec;

Mvec *mvec_alloc(Mvec *mvec, size_t cap)
{
    mvec->capacity = cap;
    void *ptr = mvec->things;
    mvec->things = memcpy(malloc(mvec->sizeof_element * cap), ptr, mvec->len * mvec->sizeof_element);
    return mvec;
}

Mvec *mvec_create(size_t sizeof_element)
{
    Mvec *mvec = malloc(sizeof(Mvec));
    mvec->len = 0;
    mvec->sizeof_element = sizeof_element;
    mvec_alloc(mvec, 2);
    return mvec;
}

void *mvec_pop(Mvec *mvec)
{
    if (mvec->len == 0) {
        return 0;
    }
    void *element = memcpy(
            malloc(mvec->sizeof_element),
            ((char *)mvec->things) + (mvec->len - 1) * mvec->sizeof_element,
            mvec->sizeof_element);
    mvec->len -= 1;
    return element;
}

void *mvec_get_at(Mvec *mvec, size_t pos)
{
    if (pos >= mvec->len) {
        return 0;
    }
    return (char *)mvec->things + mvec->sizeof_element * pos;
}

void mvec_push(Mvec *mvec, void* el)
{
    if (mvec->capacity - mvec->len <= 0) {
        mvec_alloc(mvec, mvec->capacity * 2);
    }
    memcpy((char *)mvec->things + mvec->len * mvec->sizeof_element,
            el,
            mvec->sizeof_element);
    mvec->len += 1;
}

#endif //SHIZX_MVEC_H
