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
#include <unistd.h>
#include <sys/stat.h>
#include <string.h>

#include <iostream>

#include <smart.smake.Target.h>

#include <smart.structure.Section.h>

using namespace std;
using namespace nemesis;
using namespace smart;

smake::Target::Target (const string& targetName, const string& expression) :
   a_targetName (targetName),
   a_expression (expression),
   a_gmakeTarget (targetName.c_str ())
{
}

//-------------------------------------------------------------------------------------------
// (1) Aplana la estructura de secciones que describen la estructura de datos que forman
//     los distintos TARGET's y se queda con los que afectan al nombre final del TARGET.
//-------------------------------------------------------------------------------------------
void smake::Target::initialize (const structure::Section* section)
   throw (RuntimeException)
{
   add (section);

   structure::Section::const_resource_iterator ii, maxii;
   const structure::Resource* resource;
   
   for (ii = section->resource_begin (), maxii = section->resource_end (); ii != maxii; ii ++) {
      resource = structure::Section::resource (ii);
      
      if (resource->isASection () == true)
         initialize (static_cast <const structure::Section*> (resource));
      else if (resource->onTarget () == true)
         add (resource);         
   }
   
   a_gmakeTarget.getValue ();   
}

//--------------------------------------------------------------------------------------------------------
// (1) El a_expression pueder tener un aspecto como "_{GMAKE_TARGET}.X" por lo que hay que quitar
// las llaves 
//--------------------------------------------------------------------------------------------------------
string smake::Target::calculeDirectory (const string& value) const 
   throw (RuntimeException)
{
   string result ("../");
   result += a_expression;
   
   string::size_type pos = result.find (a_targetName);
   
   if (pos == string::npos) {
      string msg ("smake::Target::calculeTargetDirectory | ");
      msg += a_expression;
      msg += " | Debe contener la cadena '";
      msg += a_targetName;
      msg += "'";
      throw RuntimeException (msg, FILE_LOCATION);
   }
      
   return result.replace (pos - 1, a_targetName.length () + 2, value);                // (1)
}
