
VALGRIND_memcheck_FLAGS = --leak-check=full --track-origins=yes
@VALGRIND_CHECK_RULES@

TESTS = circbuf_test circbuf_test_failures
check_PROGRAMS = circbuf_test circbuf_test_failures

circbuf_test_SOURCES = circbuf_test.c $(top_builddir)/src/circbuf.h
circbuf_test_CFLAGS = @CHECK_CFLAGS@ -I../vector/src/
circbuf_test_LDADD = $(top_builddir)/src/libcircbuf.la @CHECK_LIBS@

circbuf_test_failures_SOURCES = circbuf_test_failures.c $(top_builddir)/src/circbuf.h
circbuf_test_failures_CFLAGS = @CHECK_CFLAGS@ -I../vector/src/
circbuf_test_failures_LDADD = $(top_builddir)/src/libcircbuf.la @CHECK_LIBS@


debug-circbuf-test: $(top_builddir)/src/libcircbuf.la circbuf_test
	LD_LIBRARY_PATH=./vector/src/.libs:../src/.libs:/usr/local/lib CK_FORK=no gdb -tui .libs/circbuf_test
