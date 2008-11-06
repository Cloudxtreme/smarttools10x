INCLUDES=-I${NEMESIS_HOME}/include/core -I${NEMESIS_HOME}/include/io 

has_resources_SOURCES=../has_resources.cc

bin_PROGRAMS = has_resources

has_resources_LDADD=${NEMESIS_HOME}/lib/libNemesis.io.a ${NEMESIS_HOME}/lib/libNemesis.core.a -lz  -lrt

CFLAGS=-DAUTOGENERATE=1

#AM_CXXFLAGS=-DAUTOGENERATE -fno-rtti
AM_CXXFLAGS=-DAUTOGENERATE 

includedir=$(prefix)/bin/has_resources

