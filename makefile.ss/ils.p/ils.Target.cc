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

#include <unistd.h>
#include <sys/stat.h>
#include <string.h>

#include <iostream>

#include <smart.ils.Target.h>

#include <smart.structure.Section.h>

using namespace std;
using namespace nemesis;
using namespace smart;

ils::Target::Target (const char* object, const char* targetName, const char* expression) :    
   a_object (object),
   a_targetName (targetName),
   a_expression (expression),
   a_gmakeTarget (targetName)
{
}

//-------------------------------------------------------------------------------------------
// (1) Aplana la estructura de secciones que describen la estructura de datos que forman
//     los distintos TARGET's y se queda con los que afectan al nombre final del TARGET.
//-------------------------------------------------------------------------------------------
void ils::Target::initialize (const structure::Section* section)
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

bool ils::Target::link (const std::string& candidateDirectory) 
   throw ()
{
   Second candidateTime;
   
   string candidate (candidateDirectory);
   string thisObject = calculeDirectory (a_gmakeTarget.getValue ());
   
   if (candidate == thisObject)
      return false;
            
   candidate += '/';
   candidate += a_object;  

   thisObject += '/';
   thisObject += a_object;
   
   cout << "[ ILS ] Comprobando " << candidate << " ... ";

   if ((candidateTime = getModificationTime (candidate)) == -1) {
      cout << " No existe" << endl;
      return false;
   }

   struct stat data;   
   Second myTime (-1);

   if (lstat (thisObject.c_str (), &data) == 0) {
      if (S_ISLNK (data.st_mode) != 0) 
         myTime = data.st_mtime; 
      else {
         cout << "Fichero ya existe" << endl << endl;
         return false;
      }
   }
   
   if (myTime == -1) {
      if (::symlink (candidate.c_str (), thisObject.c_str ()) != 0) {
         cout << "[ ILS ] symlink (" << thisObject << "): " << strerror (errno) << endl;
         return false;
      }
      cout << "Enlace creado" << endl << endl;
   }
   else {
      if (candidateTime > myTime) {
         if (::unlink (thisObject.c_str ()) != 0) {
            cout << "[ ILS ] unlink: " << strerror (errno) << endl;
            return false;
         }
         
         if (::symlink (candidate.c_str (), thisObject.c_str ()) != 0) {
            cout << "[ ILS ] symlink-2 (" << thisObject << "): " << strerror (errno) << endl;
            return false;
         }
         
         cout << "Enlace actualizado" << endl << endl;
      }
      else 
         cout << "Enlace creado previamente" << endl << endl;
   }
   
   return true;
}

Second ils::Target::getModificationTime (const std::string& filename)
   throw ()
{
   Second result (-1);
   struct stat data;   
   
   if (stat (filename.c_str (), &data) == 0) {
      if (S_ISLNK (data.st_mode) != 0) {
         if (lstat (filename.c_str (), &data) == 0) 
            result = data.st_mtime; 
      }
      else
         result = data.st_mtime; 
   }   
   
   return result;
}

//--------------------------------------------------------------------------------------------------------
// (1) El a_expression pueder tener un aspecto como "_{GMAKE_TARGET}.X" por lo que hay que quitar
// las llaves 
//--------------------------------------------------------------------------------------------------------
string ils::Target::calculeDirectory (const string& value) const 
   throw (RuntimeException)
{
   string result ("../");
   result += a_expression;
   
   string::size_type pos = result.find (a_targetName);
   
   if (pos == string::npos) {
      string msg ("ils::Target::calculeTargetDirectory | ");
      msg += a_expression;
      msg += " | Debe contener la cadena '";
      msg += a_targetName;
      msg += "'";
      throw RuntimeException (msg, FILE_LOCATION);
   }
      
   return result.replace (pos - 1, nemesis_strlen (a_targetName) + 2, value);                // (1)
}
