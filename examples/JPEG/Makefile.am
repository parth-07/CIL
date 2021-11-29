exampledir = $(prefix)/examples/JPEG
example_PROGRAMS = remove_red_from_rgb rgb_to_grayscale
dist_example_DATA = in.jpg

AM_CPPFLAGS += -I$(top_srcdir)/include
LDADD = $(top_builddir)/lib/libcil.la

remove_red_from_rgb_SOURCES = remove_red_from_rgb.cpp
rgb_to_grayscale_SOURCES = rgb_to_grayscale.cpp
