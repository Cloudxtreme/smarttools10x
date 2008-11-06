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

#include <smart.generate.Executable.h>
#include <smart.generate.Makefile.h>

using namespace std;
using namespace nemesis;
using namespace smart;

void generate::Executable::do_writeObjectName (Makefile& makefile) const 
   throw (RuntimeException)
{
   makefile.debug (FILE_LOCATION);
   makefile.write ("OBJECT_NAME=../");
   makefile.write (description::functions::getTargetDirectory ());
   makefile.write ('/');
   makefile.write (a_node->getNameWithoutExtension ());
   makefile.newline ();   
   makefile.newline ();   
}

void generate::Executable::do_writeObjectRule (Makefile& makefile) const 
   throw (RuntimeException)
{
   makefile.debug (FILE_LOCATION);
   makefile.write ("$(OBJECT_NAME) : $(USER_LIBS) $(OBJECT_LIST)\n");
   makefile.write ("\t@echo; echo Generando ejecutable $@ ...\n");
   makefile.write ("\t$(VIEWER)$(LINKER) $(CPP_DEBUG_INDICATOR) $(CPP_FLAGS) $(USER_CPP_FLAGS) "); 
   makefile.write ("$(LINKER_OUTPUT_INDICATOR) $(OBJECT_NAME) $(OBJECT_LIST) $(USER_LIBS) $(SYSTEM_LIBS) $(LD_FLAGS) $(LIBS)\n\n");
   
   makefile.debug (FILE_LOCATION);
   makefile.writenl ("purify : ");
   makefile.write ("\t@echo; echo Generando ejecutable $(OBJECT_NAME)-purify ...\n");
   makefile.write ("\t$(VIEWER)$(PURIFY) $(USER_PURIFY_FLAGS) $(PURIFY_FLAGS) $(LINKER) $(CPP_DEBUG_INDICATOR) $(CPP_FLAGS) $(USER_CPP_FLAGS) "); 
   makefile.write ("$(LINKER_OUTPUT_INDICATOR) $(OBJECT_NAME)-purify $(OBJECT_LIST) $(USER_LIBS) $(SYSTEM_LIBS) $(LD_FLAGS) $(LIBS)\n\n");
   
   makefile.debug (FILE_LOCATION);
   makefile.writenl ("quantify : ");
   makefile.write ("\t@echo; echo Generando ejecutable $(OBJECT_NAME)-quantify ...\n");
   makefile.write ("\t$(VIEWER)$(QUANTIFY) $(USER_QUANTIFY_FLAGS) $(QUANTIFY_FLAGS) $(LINKER) $(CPP_DEBUG_INDICATOR) $(CPP_FLAGS) $(USER_CPP_FLAGS) "); 
   makefile.write ("$(LINKER_OUTPUT_INDICATOR) $(OBJECT_NAME)-quantify $(OBJECT_LIST) $(USER_LIBS) $(SYSTEM_LIBS) $(LD_FLAGS) $(LIBS)\n\n");
}
