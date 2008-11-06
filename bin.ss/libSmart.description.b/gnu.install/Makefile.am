INCLUDES=-I../hdrs -I../../libSmart.analysis.b/hdrs -I../../libSmart.analysis.b/filesystem.db/hdrs -I${prefix}/include/core -I${NEMESIS_HOME}/include/core -I${NEMESIS_HOME}/include/xml 

libSmart_description_a_SOURCES=../description.functions.cc ../description.Rule.cc ../description.sccs.cc ../description.RuleAgent.cc ../description.MetaCommand.cc ../description.Target.cc ../description.MetaLine.cc 

include_HEADERS=../hdrs/smart.description.functions.h ../hdrs/smart.description.Rule.h ../hdrs/smart.description.Target.h ../hdrs/smart.description.RuleAgent.h ../hdrs/smart.description.MetaCommand.h ../hdrs/smart.description.MetaLine.h ../hdrs/smart.description.sccs.h 

lib_LIBRARIES=libSmart.description.a

CFLAGS=-DAUTOGENERATE=1

#AM_CXXFLAGS=-DAUTOGENERATE -fno-rtti
AM_CXXFLAGS=-DAUTOGENERATE 

includedir=$(prefix)/include/description

