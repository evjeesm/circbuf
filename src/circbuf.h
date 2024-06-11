#ifndef _CIRCBUF_H_
#define _CIRCBUF_H_

#include <stdbool.h> /* bool */
#include <stddef.h>  /* size_t */

#include "vector.h"

typedef vector_t circbuf_t;

typedef struct
{
    size_t initial_cap; /* in bytes */
}
circbuf_opts_t;

typedef vector_status_t circbuf_status_t;


#define circbuf_create(...) \
    circbuf_create_(&(circbuf_opts_t){ \
        .initial_cap = 1024, \
        __VA_ARGS__ \
    })


/*
* Create circbuf according to opts.
*/
circbuf_t *circbuf_create_(const circbuf_opts_t *const opts);


/*
* Makes a copy of the circbuf.
*/
circbuf_t *circbuf_clone(const circbuf_t *const circbuf);


/*
* Free resources of the circbuf.
*/
void circbuf_destroy(circbuf_t *const circbuf);


/*
* Adjust circbuf capacity.
*/
circbuf_status_t circbuf_resize(circbuf_t **const circbuf, const size_t capacity);


/*
* Returns current capacity of the circbuf.
*/
size_t circbuf_capacity(const circbuf_t *const circbuf);


/*
* Discard contents of the circbuf.
*/
void circbuf_clear(circbuf_t *const circbuf);


/*
* Returns amount of bytes available for read operation from circbuf.
*/
size_t circbuf_avail_to_read(const circbuf_t *const circbuf);


/*
* Returns amount of bytes available for write operation into circbuf.
*/
size_t circbuf_avail_to_write(const circbuf_t *const circbuf);


/*
* Read `size` amount of bytes from the circbuf.
* Returns amout of bytes read.
* `out` buffer has to be at least `size` long.
*/
size_t circbuf_read(circbuf_t *const circbuf, const size_t size, void *const out);


/*
* Write `size` amount of bytes into the circbuf.
* Returns amout of bytes wrote.
* `in` buffer has to be at least `size` long.
*/
size_t circbuf_write(circbuf_t *const circbuf, const size_t size, void *const in);


/*
* Discard number of bytes stored in the circbuf.
*/
size_t circbuf_skip(circbuf_t *const circbuf, const size_t size);


#endif/*_CIRCBUF_H_*/
