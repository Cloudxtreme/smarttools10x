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
   
#ifndef _smart_naming_FileClass_h
#define _smart_naming_FileClass_h

#include <nemesis.SafeSortedVector.h>
#include <nemesis.defines.h>

namespace smart {

namespace naming {

class File;

using namespace nemesis;

class FileClass {
   struct SortByKey;
   friend struct SortByKey;   
   
   struct SortByKey {
      static const std::string& value (const FileClass* fileClass) throw () { return fileClass->a_id; }
   };
   
public:   
   static const FileClass* create (const char* ext) throw (RuntimeException);
   static const FileClass* instance (const File&) throw ();
   static const FileClass* instance (const char* ext) throw ();
   
   const std::string& getId () const throw () { return a_id; }
   
   std::string asString () const throw ();

private:
   const std::string a_id;

   typedef SafeSortedVector <FileClass, SortByKey, const std::string&> class_container;

   static class_container st_classes;
   
   FileClass (const std::string& id) : a_id (id) {;}
   FileClass (const FileClass&);
   FileClass ();
};

}
}

#endif
