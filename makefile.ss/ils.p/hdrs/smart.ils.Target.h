/*
   ILS - Version 10.0

   Desarrollado por la division de Nuevos conceptos de Plataformas.
   (c) Copyright 2005-2008 Telefonica Investigacion y Desarrollo. S.A. Unipersonal.

   Intelligent Linkage System 

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

#ifndef _smart_ils_Target_h
#define _smart_ils_Target_h

#include <vector>

#include <nemesis.defines.h>
#include <nemesis.RuntimeException.h>

#include <smart.Variable.h>

#include <smart.ils.ResourceContainer.h>

namespace smart {

namespace ils {

using namespace nemesis;

class Target : public ResourceContainer {
public:
   Target (const char* object, const char* targetName, const char* expression);
      
   void initialize (const structure::Section*) throw (RuntimeException);
   bool link (const std::string& candidate) throw ();

   const std::string& getValue () const throw (RuntimeException) { return a_gmakeTarget.getValue (); }
   
   std::string calculeDirectory (const std::string& targetValue) const throw (RuntimeException);
   
private:
   const char* a_object;
   const char* a_targetName;
   const char* a_expression;
   Variable a_gmakeTarget;

   static Second getModificationTime (const std::string& filename) throw ();
   
};

}
}

#endif

