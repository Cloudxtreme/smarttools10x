/*
   smake - Version 10.0

   Desarrollado por la division de Nuevos conceptos de Plataformas.
   (c) Copyright 2005-2008 Telefonica Investigacion y Desarrollo. S.A. Unipersonal.

   SmartMake, genera los objetos de librerías y ejecutables contenidos en un 
   proyecto determinado. Combina los distintos recursos disponibles para intentar
   optimizar el rendimiento mediante el ILS.

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

/*
   smake - Version 10.
   Herramienta que genera las combinaciones de TARGET's precisas para optimizar el rendimiento del ILS.

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
#ifndef _smart_smake_Shuffer_h
#define _smart_smake_Shuffer_h

#include <smart.Variable.h>

#include <smart.smake.ResourceContainer.h>

namespace smart {

namespace smake {

class Target;

class Shuffler : public ResourceContainer {
public:   
   typedef std::pair<std::string, std::string> request;
   typedef std::vector <request> request_container;
   typedef request_container::iterator request_iterator;
   typedef int iterator;

   Shuffler (Target& target) : a_target (target) {;}

   void initialize (const char* makefile_resource) throw (RuntimeException);

   bool prepareEnvironment (const iterator& ii) throw (RuntimeException);

   iterator begin () const throw () { return 0; }
   iterator end () const throw () { return 1 << resource_size (); }

   static void setEnvironment (const std::string& variable, const char* value) throw (RuntimeException);

private:
   const Target& a_target;
   request_container a_requests;
   
   request_iterator request_begin () throw () { return a_requests.begin (); }
   request_iterator request_end () throw () { return a_requests.end (); }
   request_iterator request_find (const std::string& name) throw ();
   static std::string& request_name (request_iterator& ii) throw () { return ii->first; }
   static std::string& request_value (request_iterator& ii) throw () { return ii->second; }         

   static void loadRequests (request_container&, const char* makefile_resource) throw (RuntimeException);
   static bool match (const std::string& name, const std::string& reqValue, const std::string& value) throw ();
};

}
}

#endif

