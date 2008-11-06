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

#ifndef _smart_ils_Shuffer_h
#define _smart_ils_Shuffer_h

#include <smart.Variable.h>

#include <smart.ils.ResourceContainer.h>

namespace smart {

namespace ils {

class Target;

class Shuffler : public ResourceContainer {
public:
   typedef std::vector <std::string> request_container;
   typedef request_container::iterator request_iterator;
   typedef int iterator;

   Shuffler (Target& target) : a_target (target) {;}

   void initialize (const char* makefile_resource) throw (RuntimeException);

   std::string getTargetDirectory (const iterator& ii) throw (RuntimeException);

   iterator begin () const throw () { return 0; }
   iterator end () const throw () { return 1 << resource_size (); }
   
private:
   const Target& a_target;
   
   void loadRequests (request_container&, const char* makefile_resource) throw (RuntimeException);
};

}
}

#endif

