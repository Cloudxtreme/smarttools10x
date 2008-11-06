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
#ifndef _smart_generate_SourceContainer_h_
#define _smart_generate_SourceContainer_h_

#include <smart.generate.Structure.h>

namespace smart {

namespace structure {
   class Section;
}

namespace generate {

class Makefile;

class SourceContainer : public Structure {
protected:
   SourceContainer (const Type::_v type, const bool canContainsMakefileConfig) : 
      Structure (type),
      a_canContainsMakefileConfig (canContainsMakefileConfig),
      a_containsMakefileConfig (false)
   {;}

   const_dependence_iterator object_begin () const throw () { return a_objects.begin (); }
   const_dependence_iterator object_end () const throw () { return a_objects.end (); }

   virtual void do_writeObjectName (Makefile&) const throw (RuntimeException) = 0;
   virtual void do_writeObjectRule (Makefile&) const throw (RuntimeException) = 0;
   virtual void do_writeLoad (Makefile&) const throw (RuntimeException);
   virtual void do_writeILS (Makefile&) const throw (RuntimeException) {;}

private:
   const bool a_canContainsMakefileConfig;
   bool a_containsMakefileConfig;
   dependence_container a_objects;   
   
   static std::string st_includes;

   void initialize (Prototype*, const analysis::filesystem::Node*) throw (RuntimeException);
   void beginning (Makefile&) throw (RuntimeException);
   virtual void body (Makefile&) throw (RuntimeException);
   void ending (Makefile&) throw (RuntimeException);
   void clear () throw ();

   void writeIncludes (Makefile&) const throw (RuntimeException);
   void writeAllDependences (Makefile&) throw (RuntimeException);  
   void writeObjectList (Makefile&) const throw (RuntimeException);

   virtual void do_writeDirectoryOutput (Makefile& makefile) throw (RuntimeException);

   static void forward (const analysis::filesystem::Path*, dependence_container&) throw ();
   static void forward (const structure::Section*, const structure::Section*) throw (RuntimeException);
   
   friend class Agent;
};

}
}

#endif

