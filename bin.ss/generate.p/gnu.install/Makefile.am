INCLUDES=-I../hdrs -I${prefix}/include/analysis -I${prefix}/include/core -I${prefix}/include/description -I${prefix}/include/structure -I${NEMESIS_HOME}/include/core -I${NEMESIS_HOME}/include/xml -I${NEMESIS_HOME}/include/io -I${NEMESIS_HOME}/include/app


generate_SOURCES=../generate.Library.cc ../generate.Makefile.cc ../generate.LogicalGroup.cc ../main.cc ../generate.Command.cc ../generate.defines.cc ../generate.sccs.cc ../generate.Prototype.cc ../generate.SourceContainer.cc ../generate.Agent.cc ../generate.FileSystem.cc ../generate.Structure.cc ../generate.Executable.cc ../generate.Application.cc 

bin_PROGRAMS = generate

generate_LDADD=${prefix}/lib/libSmart.analysis.a ${prefix}/lib/libSmart.description.a ${prefix}/lib/libSmart.structure.a ${prefix}/lib/libSmart.core.a ${NEMESIS_HOME}/lib/libNemesis.app.a ${NEMESIS_HOME}/lib/libNemesis.xml.a ${NEMESIS_HOME}/lib/libNemesis.io.a ${NEMESIS_HOME}/lib/libNemesis.core.a  -lxml2 -lz  -lrt

#LDADD=/usr/lib/libxml2.a -lz  -lrt

CFLAGS=-DAUTOGENERATE=1

#AM_CXXFLAGS=-DAUTOGENERATE -fno-rtti
AM_CXXFLAGS=-DAUTOGENERATE 

includedir=$(prefix)/bin/generate

