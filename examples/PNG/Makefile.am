exampledir = $(prefix)/examples/PNG
example_PROGRAMS = noRedFilter flipImage
dist_example_DATA = in.png

AM_CPPFLAGS += -I$(top_srcdir)/include
LDADD = $(top_builddir)/lib/libcil.la

noRedFilter_SOURCES = noRedFilter.cpp
flipImage_SOURCES = flipImage.cpp
