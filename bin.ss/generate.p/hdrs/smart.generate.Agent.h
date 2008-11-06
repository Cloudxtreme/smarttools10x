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
#ifndef _smart_generate_Agent_h
#define _smart_generate_Agent_h

#include <nemesis.SafeRecycler.h>

namespace smart {

namespace analysis {
   namespace filesystem {
      class Node;
   }
}

namespace generate {

class Prototype;
class FileSystem;
class LogicalGroup;
class Library;
class Executable;
class Command;

using namespace nemesis;

class Agent {
public:  
   static Prototype* instance (Prototype* predecessor, const analysis::filesystem::Node*) 
      throw (RuntimeException);
   static void release (Prototype*) throw ();

private:
   typedef SafeRecycler <FileSystem> filesystem_container;
   typedef SafeRecycler <LogicalGroup> logicalgroup_container;
   typedef SafeRecycler <Library> library_container;
   typedef SafeRecycler <Executable> executable_container;
   typedef SafeRecycler <Command> command_container;
   
   static filesystem_container st_filesystems;
   static logicalgroup_container st_logicalgroups;
   static library_container st_libraries;
   static executable_container st_executables;
   static command_container st_commands;
};

}
}

#endif

