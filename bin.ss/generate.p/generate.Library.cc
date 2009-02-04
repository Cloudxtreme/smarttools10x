/*
   generate - Version 10.
   Herramienta que crea los archivos de dependencias necesarios para crear de forma optima los ejecutables.

   Desarrollado por la division de Nuevos conceptos de Plataformas.
   (c) Copyright 2005-2007 Telefonica Investigacion y Desarrollo. S.A. Unipersonal. 

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.
   
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>. 
*/
#include <smart.description.functions.h>

#include <smart.analysis.filesystem.Node.h>

#include <smart.description.functions.h>

#include <smart.generate.Library.h>
#include <smart.generate.Makefile.h>

using namespace std;
using namespace nemesis;
using namespace smart;

void generate::Library::do_writeObjectName (Makefile& makefile) const 
   throw (RuntimeException)
{
   makefile.debug (FILE_LOCATION);
   makefile.write ("OBJECT_NAME=../");
   makefile.write (description::functions::getTargetDirectory ());
   makefile.write ('/');
   makefile.write (a_node->getNameWithoutExtension ());
   makefile.write ("$(LIBRARY_EXTENSION)\n\n");
}

void generate::Library::do_writeILS (Makefile& makefile) const 
   throw (RuntimeException)
{
   makefile.debug (FILE_LOCATION);
   makefile.writenl ("ILS :");
   makefile.write ("\t$(VIEWER)$(GMAKE_HOME)/bin/$(GMAKE_SYSTEM)/ils -r ");
   makefile.write (description::functions::getMakeResource ());
   makefile.write (" -d ");
   makefile.write (a_node->getName ());
   makefile.write (" -ce $(LIBRARY_EXTENSION) -n ");
   makefile.write (description::functions::getTargetName ());
   makefile.write (" -e ");
   makefile.write (description::functions::getTargetPrefix ());
   makefile.write ("{");
   makefile.write (description::functions::getTargetName ());
   makefile.write ("}");
   makefile.write (description::functions::getTargetExtension ());
   makefile.write ("\n\n");
}

void generate::Library::do_writeObjectRule (Makefile& makefile) const 
   throw (RuntimeException)
{
   static const char* cxx = "$(CPP) $(LIBRARY_MODE) $(USER_INCLUDES) $(SYSTEM_INCLUDES) $(CPP_DEBUG_INDICATOR) $(CPP_FLAGS) $(USER_CPP_FLAGS) -o $(OBJECT_NAME) ";
   
   makefile.debug (FILE_LOCATION);
   makefile.write ("$(OBJECT_NAME) : $(OBJECT_LIST)\n");
   makefile.write ("\t@echo; echo Generando libreria $@ ...\n");   
   makefile.tab ();
   makefile.write ("$(VIEWER)if test \"$(GMAKE_LIBRARY)\" = \"dynamic\" ; \\\n\tthen \\\n");
   makefile.tab (2);
      makefile.write (cxx);
      makefile.write ("$(OBJECT_LIST) $(USER_LIBS) $(SYSTEM_LIBS) $(LIBS) ; \\\n");
   makefile.tab ();
   makefile.write ("else \\\n");
   makefile.tab (2);
      makefile.write ("if test \"$(CC_USE_AR)\" = \"true\" ; \\\n\t\tthen \\\n");
      makefile.tab (3);
         makefile.write ("$(AR) $(AR_FLAGS) $(OBJECT_NAME) $(OBJECT_LIST) ; \\\n");
      makefile.tab (2);
      makefile.write ("else \\\n");
      makefile.tab (3);
         makefile.write (cxx);
         makefile.write ("$(OBJECT_LIST) ; \\\n");
      makefile.tab (2);
      makefile.write ("fi ; \\\n");
   makefile.tab ();
   makefile.write ("fi ; \\\n");

   makefile.write ("\tif [ \"$(CC_NEED_CHMOD)\" = \"true\" ] ; \\\n\tthen \\\n");
   makefile.write ("\t\tchmod -R ug+rwx ");
   makefile.write (description::functions::getTargetDirectory ());
   makefile.write (" ; \\\n\tfi ;\n\t@echo; echo\n\n");
}

void generate::Library::do_writeDirectoryOutput (Makefile& makefile) 
   throw (RuntimeException)
{
   makefile.debug (FILE_LOCATION);
   makefile.write ('%');
   makefile.write (description::functions::getTargetExtension ());
   makefile.writenl (": ");
   makefile.writenl ("\t$(VIEWER)$(GMAKE_HOME)/bin/$(GMAKE_SYSTEM)/ils_mkdir -d $@ -o $(OBJECT_NAME)\n");
}
