SUBDIRS = 

if JPEG
SUBDIRS += JPEG
endif

if PNG
SUBDIRS += PNG
endif

AM_CPPFLAGS += -I$(top_srcdir)/include

lib_LTLIBRARIES = libcil.la

libcil_la_SOURCES = ImageInfo.cpp ImageMatrix.cpp Pixel.cpp
libcil_la_LIBADD =

if JPEG
libcil_la_LIBADD += JPEG/libjpegcil.la
endif

if PNG
libcil_la_LIBADD += PNG/libpngcil.la
endif
