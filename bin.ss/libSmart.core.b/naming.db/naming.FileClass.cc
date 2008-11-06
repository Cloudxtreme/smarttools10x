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
   
#include <nemesis.Guard.h>

#include <smart.naming.FileClass.h>
#include <smart.naming.File.h>

using namespace std;
using namespace nemesis;
using namespace smart;

naming::FileClass::class_container naming::FileClass::st_classes;

const naming::FileClass* naming::FileClass::instance (const File& fileName)
   throw ()
{
   const FileClass* result (NULL);   
   
   const std::string& name = fileName.getName ();   
   string::size_type pos = name.find_last_of ('.');
   
   if (pos != string::npos) {   
      const char* id = name.c_str () + pos + 1;            
      
      if (*id != 0) {
         Guard guard (st_classes, "class_container from smart::naming::FileClass::instance");  
         string sid (id);
         result = st_classes.find (sid);
      }
   }

   return result;
}

const naming::FileClass* naming::FileClass::instance (const char* id) 
   throw ()
{
   const FileClass* result (NULL);   

   if (id != NULL && *id != 0) {
      Guard guard (st_classes, "class_container from smart::naming::FileClass::instance");  
      string sid (id);
      result = st_classes.find (sid);
   }
   
   return result;
}

const naming::FileClass* naming::FileClass::create (const char* ext) 
   throw (RuntimeException)
{
   Guard guard (st_classes, "class_container from smart::naming::FileClass::create");  
   FileClass* result (NULL);

   string id (ext);

   if ((result = st_classes.find (id)) == NULL) 
      st_classes.add (result = new FileClass (id));
   
   return result;
}

string naming::FileClass::asString () const 
   throw ()
{
   string result ("smart::naming::FileClass { Value: ");
   result += a_id;
   return result += " }";
}

