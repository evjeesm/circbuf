noinst_LTLIBRARIES = libcircbuf_funcs.la
lib_LTLIBRARIES = libcircbuf.la libcircbuf_static.la

libcircbuf_funcs_la_SOURCES = circbuf.c circbuf.h
libcircbuf_funcs_la_CFLAGS = -I../vector/src/
libcircbuf_funcs_la_LIBADD = ../vector/src/libvector.la

libcircbuf_la_SOURCES =
libcircbuf_la_LDFLAGS = -shared
libcircbuf_la_LIBADD = libcircbuf_funcs.la ../vector/src/libvector.la

libcircbuf_static_la_SOURCES =
libcircbuf_static_la_LDFLAGS = -static
libcircbuf_static_la_LIBADD = libcircbuf_funcs.la ../vector/src/libvector_static.la
