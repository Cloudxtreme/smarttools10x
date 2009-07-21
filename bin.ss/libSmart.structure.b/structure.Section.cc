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
#include <nemesis.Logger.h>
#include <nemesis.defines.h>

#include <smart.structure.Section.h>

using namespace std;
using namespace smart;

structure::Section::~Section ()
{
   for (resource_iterator ii = resource_begin (), maxii = resource_end (); ii != maxii; ii ++) 
      delete resource (ii);
      
   a_resources.clear ();   
}

void structure::Section::add (Resource* resource) 
   throw ()
{
   LOGINFORMATION (
      string msg ("structure::Section::add | Section: ");
      msg += getName ();
      msg += " | Resource: ";
      msg += resource->getName ();
      Logger::information (msg, FILE_LOCATION);
   );

   resource->setParent (this);   
   a_resources.push_back (resource);
}

string structure::Section::serialize () const
   throw ()
{
   string result (Resource::getName ());
   bool first = true;
   
   result += " {";
   
   for (const_resource_iterator ii = resource_begin (), maxii = resource_end (); ii != maxii; ii ++) {
      if (first == true) 
         first = false;
      else
         result += ' ';
      
      result += resource (ii)->serialize ();        
   }
   
   return result += " }";
}

   

