/*
   Smart.structure - Version 10.
   Libreria para gestionar la estructura de resource/vendor/version implicados a la hora de 
   la generacion de ejecutables. 

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
#ifndef _smart_structure_Resource_h
#define _smart_structure_Resource_h

#include <vector>

#include <nemesis.RuntimeException.h>

namespace smart {

class Variable;

namespace structure {

using namespace nemesis;

class Resource {
public:
   Resource (const std::string& name, const bool onTarget, const bool enableILS, const bool isASection = false);
   virtual ~Resource ();

   bool isASection () const throw () { return a_isASection; }
   const std::string& getName () const throw () { return a_name; }
   bool onTarget () const throw () { return a_onTarget; }
   bool enableILS () const throw () { return a_enableILS; }
   const Variable* getVariable () const throw () { return a_variable; }
   const Resource* getParent () const throw () { return a_parent; }

   void setParent (const Resource* parent) throw () { a_parent = parent; }

   virtual std::string serialize () const throw ();
   
private:
   const bool a_isASection;
   const std::string a_name;
   const bool a_onTarget;
   const bool a_enableILS;
   Variable* a_variable;
   const Resource* a_parent;
};

}
}

#endif
