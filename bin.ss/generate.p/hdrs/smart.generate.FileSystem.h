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
#ifndef _smart_generate_FileSystem_h_
#define _smart_generate_FileSystem_h_

#include <smart.generate.Structure.h>

namespace smart {

namespace generate {

class Makefile;

class FileSystem : public Structure {
public:
   FileSystem () : Structure (Type::FileSystem) {;}

private:
   dependence_container a_libraries;
   dependence_container a_execs;   
   std::string a_libraryExtensions;
   std::string a_execExtensions;

   void initialize (Prototype* predecessor, const analysis::filesystem::Node*) throw (RuntimeException);
   void body (Makefile&) throw (RuntimeException);
   void clear () throw ();
   void copy (const Prototype* other) throw ();

   void prepareExtensions ()throw ();

   static void forward (const analysis::filesystem::Path*, dependence_container& libs, dependence_container& execs) throw ();
   
   friend class Agent;
};

}
}

#endif

