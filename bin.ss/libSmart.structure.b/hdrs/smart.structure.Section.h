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
#ifndef _smart_structure_Section_h
#define _smart_structure_Section_h

#include <smart.structure.Resource.h>

namespace smart {

class Variable;

namespace structure {

using namespace nemesis;

class Section : public Resource {
public:
   typedef std::vector <Resource*> resource_container;
   typedef resource_container::iterator resource_iterator;
   typedef resource_container::const_iterator const_resource_iterator;

   Section (const std::string& name) : Resource (name, true, false, true) {;}
   ~Section ();

   void add (Resource* resource) throw ();

   resource_iterator resource_begin () throw () { return a_resources.begin (); }
   resource_iterator resource_end () throw () { return a_resources.end (); }

   const_resource_iterator resource_begin () const throw () { return a_resources.begin (); }
   const_resource_iterator resource_end () const throw () { return a_resources.end (); }

   std::string serialize () const throw ();
      
   static Resource* resource (resource_iterator& ii) throw () { return *ii; }
   static const Resource* resource (const_resource_iterator& ii) throw () { return *ii; }
      
private:
   resource_container a_resources;
};

}
}

#endif
