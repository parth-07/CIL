noinst_LTLIBRARIES = libpngcil.la

PKGLIBS = libpng

AM_CPPFLAGS += -I$(top_srcdir)/include

libpngcil_la_SOURCES = PNGCore.cpp PNGHandler.cpp
libpngcil_la_CXXFLAGS = $(shell pkg-config --cflags $(PKGLIBS))
libpngcil_la_LDFLAGS = $(shell pkg-config --libs $(PKGLIBS))
