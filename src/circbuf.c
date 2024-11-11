#include "circbuf.h"

#include <assert.h> /* asserts */
#include <string.h> /* memcpy */

#define WORDSIZE sizeof(size_t)

typedef struct
{
    size_t ext_header_size;
    size_t read;
    size_t write;
}
circbuf_header_t;


/**                          ***
* === Forward Declarations === *
**                            */

static circbuf_header_t *get_circbuf_header(const circbuf_t *const circbuf);


/**                          ***
* === API Implementation   === *
**                            */

circbuf_t *circbuf_create_(const circbuf_opts_t *const opts)
{
    assert(opts);
    assert(opts->initial_cap);

    circbuf_t *circbuf = vector_create (
        .alloc_opts = opts->alloc_opts,
        .ext_header_size = sizeof(circbuf_header_t) + opts->ext_header_size,
        .element_size = 1,
        .initial_cap = opts->initial_cap + 1,
    );

    if (!circbuf) return NULL;

    circbuf_header_t *header = get_circbuf_header(circbuf);
    *header = (circbuf_header_t) {
        .ext_header_size = opts->ext_header_size,
    };

    return circbuf;
}


circbuf_t *circbuf_clone(const circbuf_t *const circbuf)
{
    assert(circbuf);

    return vector_clone(circbuf);
}


circbuf_status_t circbuf_resize(circbuf_t **const circbuf, const size_t capacity)
{
    assert(circbuf && *circbuf);
    assert(circbuf_avail_to_read(*circbuf) <= capacity && "Discarding unread data by shrinking capacity!");

    const circbuf_header_t *header = get_circbuf_header(*circbuf);
    const size_t element_size = vector_element_size(*circbuf);

    circbuf_t *new = vector_create (
        .alloc_opts = vector_alloc_opts(*circbuf),
        .ext_header_size = header->ext_header_size,
        .element_size = element_size,
        .initial_cap = capacity + 1
    );

    if (!new) return VECTOR_ALLOC_ERROR;

    char buff[WORDSIZE];
    size_t available;
    while ((available = circbuf_avail_to_read(*circbuf)))
    {
        size_t to_read = WORDSIZE > available ? available : WORDSIZE;
        size_t bytes = circbuf_read(*circbuf, to_read, buff);
        circbuf_write(new, bytes, buff);
    }

    *circbuf = new;
    return VECTOR_SUCCESS;
}


void circbuf_destroy(circbuf_t *const circbuf)
{
    assert(circbuf);

    vector_destroy(circbuf);
}


size_t circbuf_capacity(const circbuf_t *const circbuf)
{
    assert(circbuf);

    return vector_capacity(circbuf) - 1;
}


void circbuf_clear(circbuf_t *const circbuf)
{
    assert(circbuf);

    *get_circbuf_header(circbuf) = (circbuf_header_t) {0};
}


size_t circbuf_avail_to_read(const circbuf_t *const circbuf)
{
    assert(circbuf);
    const circbuf_header_t *header = get_circbuf_header(circbuf);

    if (header->write >= header->read)
    {
        return header->write - header->read;
    }

    return (circbuf_capacity(circbuf) - header->read + 1) + header->write;
}


size_t circbuf_avail_to_write(const circbuf_t *const circbuf)
{
    assert(circbuf);

    return circbuf_capacity(circbuf) - circbuf_avail_to_read(circbuf);
}


size_t circbuf_read(circbuf_t *const circbuf, const size_t size, void *const out)
{
    assert(circbuf);
    assert(size);
    assert(size <= circbuf_avail_to_read(circbuf));
    assert(out);

    circbuf_header_t *header = get_circbuf_header(circbuf);
    const size_t capacity = circbuf_capacity(circbuf);
    const size_t bufsize = capacity + 1;

    if (header->write == header->read) return 0;
    if (header->write > header->read)
    {
        size_t length = header->write - header->read;
        length = size <= length ? size : length;
        memcpy(out, vector_get(circbuf, header->read), length);
        header->read = (header->read + length) % bufsize;
        return length;
    }
    
    /* split case */
    size_t length = bufsize - header->read;
    if (length >= size)
    {
        memcpy(out, vector_get(circbuf, header->read), size);
        header->read = (header->read + size) % bufsize;
        return size;
    }

    memcpy(out, vector_get(circbuf, header->read), length);
    size_t rest = size - length;
    rest = header->write < rest ? header->write : rest;

    memcpy((char*)out + length, vector_get(circbuf, 0), rest);
    header->read = rest;
    return length + rest;
}


size_t circbuf_write(circbuf_t *const circbuf, const size_t size, void *const in)
{
    assert(circbuf);
    assert(size);
    assert(size <= circbuf_avail_to_write(circbuf));
    assert(in);

    circbuf_header_t *header = get_circbuf_header(circbuf);
    const size_t capacity = circbuf_capacity(circbuf);
    const size_t bufsize = capacity + 1;

    if ((header->read == 0 && header->write == capacity)
      ||(header->read > 0 && header->write == header->read - 1)) return 0;

    if (header->read > header->write)
    {
        size_t length = header->read - header->write;
        length = size <= length ? size : length;
        memcpy(vector_get(circbuf, header->write), in, length);
        header->write = (header->write + length) % bufsize;
        return length;
    }

    /* split case */
    size_t length = bufsize - header->write;
    if (length >= size)
    {
        memcpy(vector_get(circbuf, header->write), in, size);
        header->write = (header->write + size) % bufsize;
        return size;
    }

    memcpy(vector_get(circbuf, header->write), in, length);
    size_t rest = size - length;
    rest = header->read < rest ? header->read : rest;

    memcpy(vector_get(circbuf, 0), (char*)in + length, rest);
    header->write = rest;
    return length + rest;
}


size_t circbuf_skip(circbuf_t *const circbuf, const size_t size)
{
    assert(circbuf);
    assert(size);
    assert(size <= circbuf_avail_to_read(circbuf));

    circbuf_header_t *header = get_circbuf_header(circbuf);
    const size_t capacity = circbuf_capacity(circbuf);
    const size_t bufsize = capacity + 1;

    if (header->write == header->read) return 0;
    if (header->write > header->read)
    {
        size_t length = header->write - header->read;
        length = size <= length ? size : length;
        header->read = (header->read + length) % bufsize;
        return length;
    }

    /* split case */
    size_t length = bufsize - header->read;
    if (length >= size)
    {
        header->read = (header->read + size) % bufsize;
        return size;
    }

    size_t rest = size - length;
    rest = header->write < rest ? header->write : rest;
    header->read = rest;
    return length + rest;
}


/**                       ***
* === Static Functions  === *
**                         */

static circbuf_header_t *get_circbuf_header(const circbuf_t *const circbuf)
{
    return (circbuf_header_t*) vector_get_ext_header(circbuf);
}

