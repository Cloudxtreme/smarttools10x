INCLUDES=-I../hdrs -I../filesystem.db/hdrs -I${prefix}/include/core -I${prefix}/include/description -I${NEMESIS_HOME}/include/core -I${NEMESIS_HOME}/include/xml -I${NEMESIS_HOME}/include/io -I${NEMESIS_HOME}/include/app

libSmart_analysis_a_SOURCES=../analysis.Scanner.cc ../analysis.sccs.cc ../analysis.ASN1Source.cc ../analysis.Application.cc ../analysis.FileSystem.cc ../analysis.CSource.cc ../filesystem.db/analysis.filesystem.Path.cc ../filesystem.db/analysis.filesystem.FlatEntry.cc ../filesystem.db/analysis.filesystem.Node.cc ../filesystem.db/analysis.filesystem.FlatDirectory.cc 

include_HEADERS=../hdrs/smart.analysis.ASN1Source.h ../hdrs/smart.analysis.Scanner.h ../hdrs/smart.analysis.Application.h ../hdrs/smart.analysis.sccs.h ../hdrs/smart.analysis.FileSystem.h ../hdrs/smart.analysis.CSource.h ../filesystem.db/hdrs/smart.analysis.filesystem.Path.h ../filesystem.db/hdrs/smart.analysis.filesystem.FlatDirectory.h ../filesystem.db/hdrs/smart.analysis.filesystem.FlatEntry.h ../filesystem.db/hdrs/smart.analysis.filesystem.Node.h 

lib_LIBRARIES=libSmart.analysis.a

CFLAGS=-DAUTOGENERATE=1

#AM_CXXFLAGS=-DAUTOGENERATE -fno-rtti
AM_CXXFLAGS=-DAUTOGENERATE 

includedir=$(prefix)/include/analysis

