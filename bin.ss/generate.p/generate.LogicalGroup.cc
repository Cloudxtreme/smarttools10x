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

#include <smart.generate.LogicalGroup.h>
#include <smart.generate.Makefile.h>

using namespace std;
using namespace nemesis;
using namespace smart;

void generate::LogicalGroup::do_writeObjectName (Makefile& makefile) const 
   throw (RuntimeException)
{
   makefile.debug (FILE_LOCATION);
   makefile.write ("OBJECT_NAME=$(OBJECT_LIST)\n\n");
}


