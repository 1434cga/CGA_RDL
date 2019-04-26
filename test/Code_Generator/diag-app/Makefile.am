# Makefile.am - libdiag_app.a

common_sources:= \
	diag_app.cpp

# Target name DSO
lib_LTLIBRARIES := libdiag_app.la

libdiag_app_la_CPPFLAGS := \
	-I$(SDKTARGETSYSROOT)/usr/include/servicelayer \
	-O2 \
	-g \
	-Wall \
	-Wno-unused-parameter \
	-DHAVE_SYS_UIO_H \
	-rdynamic \
	-fpermissive

libdiag_app_la_LDFLAGS = \
	-lpthread \
	-lapplication \
	-liapplication \
	-lisystem \
	-lidiag \
	-shared \
	-module \
	-export-dynamic \
	-export-symbols-regex 'createApplication'

requiredlibs := -lpthread

libdiag_app_la_SOURCES := \
	$(common_sources)
