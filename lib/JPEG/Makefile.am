noinst_LTLIBRARIES = libjpegcil.la

PKGLIBS = libjpeg

AM_CPPFLAGS += -I$(top_srcdir)/include

libjpegcil_la_SOURCES = JPEGCore.cpp JPEGHandler.cpp
libjpegcil_la_CXXFLAGS = $(shell pkg-config --cflags $(PKGLIBS))
libjpegcil_la_LDFLAGS = $(shell pkg-config --libs $(PKGLIBS))
