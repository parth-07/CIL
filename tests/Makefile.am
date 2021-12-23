check-local:
	cd $(top_builddir)/tests
	$(LN_S) -f $(top_srcdir)/tests/results .
	$(LN_S) -f $(top_srcdir)/tests/in.png .
	$(LN_S) -f $(top_srcdir)/tests/in.jpeg .
	$(LN_S) -f $(top_srcdir)/tests/in.ppm .

AM_DEFAULT_SOURCE_EXT = .cpp
AM_CPPFLAGS += -I$(top_srcdir)/include

check_LTLIBRARIES = libciltests.la
libciltests_la_SOURCES = main.cpp
libciltests_la_CXXFLAGS = $(AM_CPPFLAGS) $(shell pkg-config --cflags catch2)

LDADD = $(top_builddir)/libcil.la libciltests.la
check_PROGRAMS = crop rotate flip resize
check_CXXFLAGS = $(AM_CPPFLAGS) $(shell pkg-config --cflags catch2)

TESTS = $(check_PROGRAMS)
