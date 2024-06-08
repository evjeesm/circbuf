ACLOCAL_AMFLAGS = -I m4 -I/usr/share/aclocal
SUBDIRS = vector vector/src src tests

debug-circbuf-test: ./src/libcircbuf.la ./tests/circbuf_test
	LD_LIBRARY_PATH=./vector/src/.libs:./src/.libs:/usr/local/lib CK_FORK=no gdb -tui ./tests/.libs/circbuf_test
