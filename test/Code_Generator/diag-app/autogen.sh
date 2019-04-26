#!/bin/sh

# autogen.sh -- Autotools bootstrapping
#

libtoolize --copy --force
aclocal ${OECORE_ACLOCAL_OPTS} &&\
autoheader &&\
autoconf &&\
automake --add-missing --copy

./configure ${CONFIGURE_FLAGS}


