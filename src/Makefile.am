noinst_LTLIBRARIES = libcircbuf_funcs.la
libcircbuf_funcs_la_SOURCES = circbuf.c circbuf.h
libcircbuf_funcs_la_CPPFLAGS = -I$(top_srcdir)/vector/src/
libcircbuf_funcs_la_LIBADD = $(top_builddir)/vector/src/libvector_static.la

lib_LTLIBRARIES = libcircbuf_static.la
if !MINGW
lib_LTLIBRARIES += libcircbuf.la
endif

libcircbuf_la_SOURCES =
libcircbuf_la_LDFLAGS = -shared
libcircbuf_la_LIBADD = libcircbuf_funcs.la \
					   $(top_builddir)/vector/src/libvector.la
libcircbuf_la_LIBS = $(CODE_COVERAGE_LIBS)
libcircbuf_la_CPPFLAGS = $(CODE_COVERAGE_CPPFLAGS)
libcircbuf_la_CFLAGS = $(CODE_COVERAGE_CFLAGS)
libcircbuf_la_CXXFLAGS = $(CODE_COVERAGE_CXXFLAGS)

libcircbuf_static_la_SOURCES =
libcircbuf_static_la_LDFLAGS = -static
libcircbuf_static_la_LIBADD = libcircbuf_funcs.la \
							  $(top_builddir)/vector/src/libvector_static.la
libcircbuf_static_la_LIBS = $(CODE_COVERAGE_LIBS)
libcircbuf_static_la_CPPFLAGS = $(CODE_COVERAGE_CPPFLAGS)
libcircbuf_static_la_CFLAGS = $(CODE_COVERAGE_CFLAGS)
libcircbuf_static_la_CXXFLAGS = $(CODE_COVERAGE_CXXFLAGS)

include_HEADERS = circbuf.h
