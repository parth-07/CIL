ACLOCAL_AMFLAGS = -I m4
SUBDIRS = include
AM_DEFAULT_SOURCE_EXT = .cpp

## library libcil
AM_CPPFLAGS += -I$(top_srcdir)/include

lib_LTLIBRARIES = libcil.la

libcil_la_SOURCES = lib/ImageInfo.cpp lib/ImageMatrix.cpp lib/Pixel.cpp lib/Transformations.cpp \
                    lib/Core/Coordinate.cpp lib/Core/DetachedFPPixel.cpp lib/Core/Utils.cpp lib/PPM/PPMHandler.cpp

libcil_la_CXXFLAGS = $(AM_CPPFLAGS)
libcil_la_LDFLAGS = $(AM_CPPFLAGS)
PKGLIBS =

if JPEG
PKGLIBS += libjpeg
libcil_la_SOURCES += lib/JPEG/JPEGCore.cpp lib/JPEG/JPEGHandler.cpp
endif

if PNG
PKGLIBS += libpng
libcil_la_SOURCES += lib/PNG/PNGCore.cpp lib/PNG/PNGHandler.cpp
endif

libcil_la_CXXFLAGS += $(shell pkg-config --cflags $(PKGLIBS))
libcil_la_LDFLAGS += $(shell pkg-config --libs $(PKGLIBS))

## example codes
if EXAMPLES
LDADD = $(top_builddir)/libcil.la

exampledir = $(prefix)/examples

dist_example_DATA = examples/scenary.ppm examples/in.jpg examples/in.png examples/bird.ppm

example_PROGRAMS = examples/remove_red_from_rgb examples/invert \
                   examples/padImage examples/edgeDetection \
                   examples/resize examples/noRedFilter \
                   examples/flipImage examples/rotateImage \
                   examples/blur examples/grayscale \
				   examples/crop

endif

