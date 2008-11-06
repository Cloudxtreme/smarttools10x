/*
   browse - Version 10.
   Ejecutable que ayuda a visualizar los archivos que dependen de un determinado archivo de cabecera o ASN1.

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
#ifndef _smart_browse_Application_h
#define _smart_browse_Application_h

#include <smart.analysis.Application.h>

namespace smart {

namespace analysis {
   namespace filesystem {
      class FlatEntry;
      class Path;
   }
}

namespace browse {

using namespace nemesis;

class Application : public analysis::Application {
public:
   Application ();
   
private:
   void run () throw (RuntimeException);      
   void process (const std::string& filename) throw ();

   static void forward (const analysis::filesystem::FlatEntry*, const analysis::filesystem::Path*) throw ();
};

}
}

#endif
