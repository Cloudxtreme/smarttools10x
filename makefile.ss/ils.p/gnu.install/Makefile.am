INCLUDES=-I../hdrs -I${prefix}/include/analysis -I${prefix}/include/core -I${prefix}/include/description -I${prefix}/include/structure -I${NEMESIS_HOME}/include/core -I${NEMESIS_HOME}/include/io -I${NEMESIS_HOME}/include/app


ils_SOURCES=../ils.Shuffler.cc ../ils.cc ../ils.Target.cc 

bin_PROGRAMS = ils

ils_LDADD=${prefix}/lib/libSmart.structure.a ${prefix}/lib/libSmart.core.a ${NEMESIS_HOME}/lib/libNemesis.io.a ${NEMESIS_HOME}/lib/libNemesis.core.a -lxml2 -lz  -lrt

#LDADD=/usr/lib/libxml2.a -lz  -lrt

CFLAGS=-DAUTOGENERATE=1

#AM_CXXFLAGS=-DAUTOGENERATE -fno-rtti
AM_CXXFLAGS=-DAUTOGENERATE 

includedir=$(prefix)/bin/ils

