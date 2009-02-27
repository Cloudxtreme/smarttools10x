INCLUDES=-I../hdrs -I../naming.db/hdrs -I${NEMESIS_HOME}/include/core  -I${NEMESIS_HOME}/include/io

libSmart_core_a_SOURCES=../core.Variable.cc ../naming.db/naming.File.cc ../naming.db/naming.FileClass.cc ../core.functions.cc ../core.sccs.cc 

include_HEADERS=../naming.db/hdrs/smart.naming.File.h ../naming.db/hdrs/smart.naming.FileClass.h ../hdrs/smart.Variable.h ../hdrs/smart.functions.h ../hdrs/smart.sccs.h 

lib_LIBRARIES=libSmart.core.a

CFLAGS=-DAUTOGENERATE=1

#AM_CXXFLAGS=-DAUTOGENERATE -fno-rtti
AM_CXXFLAGS=-DAUTOGENERATE 

includedir=$(prefix)/include/core

