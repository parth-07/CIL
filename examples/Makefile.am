AM_CPPFLAGS += -I$(top_srcdir)/include
LDADD = $(top_builddir)/lib/libcil.la
AM_DEFAULT_SOURCE_EXT = .cpp

exampledir = $(prefix)/examples
examplejpegdir = $(exampledir)/JPEG
examplepngdir = $(exampledir)/PNG

if JPEG
examplejpeg_PROGRAMS = JPEG/remove_red_from_rgb JPEG/rgb_to_grayscale
dist_examplejpeg_DATA = JPEG/in.jpg
endif

if PNG
examplepng_PROGRAMS = PNG/noRedFilter PNG/flipImage
dist_examplepng_DATA = PNG/in.png
endif

