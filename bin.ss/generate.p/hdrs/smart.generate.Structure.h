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
#ifndef _smart_generate_Structure_h
#define _smart_generate_Structure_h

#include <smart.generate.Prototype.h>

namespace smart {

namespace analysis {
   namespace filesystem {
      class Path;
   }
}

namespace generate {

class Structure : public Prototype {
public:
   Makefile& getMakefile () throw () { return *a_makefile; }

   void execute () throw (RuntimeException);

protected:  
   Makefile* a_makefile;

   Structure (const Type::_v type) : 
      Prototype (type),
      a_makefile (NULL) 
   {;}

   virtual void initialize (Prototype* predecessor, const analysis::filesystem::Node*) 
      throw (RuntimeException);
   virtual void clear () throw ();

   virtual void beginning (Makefile&) throw (RuntimeException);
   virtual void body (Makefile&) throw (RuntimeException) = 0;
   virtual void ending (Makefile&) throw (RuntimeException);
   
   void writeAllDependences (Makefile&, dependence_container&) throw (RuntimeException); 
   
   static void writeClobber (Makefile&) throw (RuntimeException);
   
private:  
   void forward (const analysis::filesystem::Path*) throw (RuntimeException);
};

}
}

#endif

