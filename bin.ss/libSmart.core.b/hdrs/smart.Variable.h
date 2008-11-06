/*
   Smart.core - Core de las aplicaciones Smart. 
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
   
#ifndef _smart_Variable_h
#define _smart_Variable_h

#include <nemesis.Variable.h>

namespace smart {

using namespace nemesis;

class Variable : public nemesis::Variable {
public:
   Variable (const char* name) : nemesis::Variable (name, a_value), a_isEnabled (true) { }
   Variable (const Variable& other) : 
      nemesis::Variable (other.getName (), a_value), 
      a_value (other.a_value),
      a_isEnabled (other.a_isEnabled) 
   { }
   
   bool isEnabled () const throw () { return a_isEnabled; }
   
   const std::string& getValue () const throw (RuntimeException);

   bool operator < (const Variable& other) const throw () {
      return a_value < other.a_value;
   }

   void disable () throw () { a_isEnabled = false; }
   
   std::string asString () const throw ();

   static void disableVerbose () throw () { st_verbose = false; }

private:
   std::string a_value; 
   bool a_isEnabled;

   static bool st_verbose;
};

}

#endif
