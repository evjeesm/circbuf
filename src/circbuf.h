#ifndef _CIRCBUF_H_
#define _CIRCBUF_H_

#include <stdbool.h> /* bool */
#include <stddef.h>  /* size_t */

#include "vector.h"

typedef vector_t circbuf_t;

typedef struct
{
    size_t initial_cap; /* in bytes */
    vector_error_callback_t error_callback;
    vector_error_t *error_out;
}
circbuf_opts_t;


#define circbuf_create(buf_ptr, ...) \
    _Pragma("GCC diagnostic push") \
    _Pragma("GCC diagnostic ignored \"-Woverride-init\"") \
    circbuf_create_(&buf_ptr, &(circbuf_opts_t){ \
        .initial_cap = 1024, \
        .error_callback = vector_default_error_callback, \
        __VA_ARGS__ \
    }); \
    _Pragma("GCC diagnostic pop")


#define circbuf_create_errhdl(buf_ptr, error_ptr, ...) do{\
    *error_ptr = DYNARR_NO_ERROR; \
    circbuf_create(buf_ptr, \
        .error_callback = vector_manual_error_callback, \
        .error_out = error_ptr, \
        __VA_ARGS__ \
    )} while (0)

/*
* Create circbuf according to opts.
*/
void circbuf_create_(circbuf_t **const circbuf, const circbuf_opts_t *const opts);


/*
* Makes a copy of the circbuf.
*/
circbuf_t *circbuf_clone(circbuf_t *const circbuf);


/*
* Free resources of the circbuf.
*/
void circbuf_destroy(circbuf_t *const circbuf);


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
