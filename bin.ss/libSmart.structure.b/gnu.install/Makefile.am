INCLUDES=-I../hdrs -I${prefix}/include/core  -I${NEMESIS_HOME}/include/core -I${NEMESIS_HOME}/include/xml

libSmart_structure_a_SOURCES=../structure.Resource.cc ../structure.Agent.cc ../structure.Section.cc ../structure.sccs.cc 

include_HEADERS=../hdrs/smart.structure.sccs.h ../hdrs/smart.structure.Agent.h ../hdrs/smart.structure.Section.h ../hdrs/smart.structure.Resource.h 

lib_LIBRARIES=libSmart.structure.a

CFLAGS=-DAUTOGENERATE=1

#AM_CXXFLAGS=-DAUTOGENERATE -fno-rtti
AM_CXXFLAGS=-DAUTOGENERATE 

includedir=$(prefix)/include/structure

