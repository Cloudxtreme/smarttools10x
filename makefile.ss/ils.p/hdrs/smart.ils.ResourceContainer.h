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

#ifndef _smart_ils_ResourceContainer_h
#define _smart_ils_ResourceContainer_h

#include <vector>

#include <nemesis.RuntimeException.h>

namespace smart {

namespace structure {
   class Resource;
   class Section;
}

namespace ils {

using namespace nemesis;

class ResourceContainer {
public:
   typedef std::vector <const structure::Resource*> resource_container;
   typedef resource_container::const_iterator const_resource_iterator;

   void add (const structure::Resource* resource) throw () { a_resources.push_back (resource); }
   
   const_resource_iterator resource_begin () const throw () { return a_resources.begin (); }
   const_resource_iterator resource_end () const throw () { return a_resources.end (); }
   int resource_size () const throw () { return a_resources.size (); }

   static const structure::Resource* resource (const_resource_iterator& ii) throw () { return *ii; }
   
private:
   resource_container a_resources;
};

}
}

#endif

