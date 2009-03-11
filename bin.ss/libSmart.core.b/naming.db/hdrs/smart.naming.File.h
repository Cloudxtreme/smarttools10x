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
   
#ifndef _smart_naming_File_h
#define _smart_naming_File_h

#include <nemesis.Mutex.h>
#include <nemesis.RuntimeException.h>

namespace smart {

class Variable;

namespace naming {

class FileClass;

class File : public nemesis::Mutex {
public:
   struct SortByName {
      static const std::string& value (const File* file) throw () { return file->a_name; }
   };

   File ();
   explicit File (File* parent);
   File (File* parent, const std::string& name);
   File (File* parent, const char* name);
   File (const File&);
   ~File ();

   const std::string& getName () const throw () { return a_name; }
   const FileClass* getFileClass () const throw () { return a_fileClass; }
   File* getParent () throw () { return a_parent; }
   const File* getParent () const throw () { return a_parent; }
   std::string getNameWithoutExtension () const throw ();

   const std::string& getFullPath () const throw ();
   const std::string& getVariablePath () const throw ();
   std::string getRelativePath (const File* from) const throw ();

   void setName (const char* name) throw ();
   void setName (const char* name, const char* ext) throw ();
   const FileClass* replaceName (const std::string& name) throw ();
   
   bool operator== (const File& other) const throw () { 
      return a_parent == other.a_parent && a_name == other.a_name;
   }
   
   File& operator= (const File& other) throw ();
   
   virtual std::string asString () const throw ();

protected:
   File (const Variable*);
   
private:
   //-----------------------------------------------------------------------
   // a_parent: Elemento del que depende
   // a_name: Nombre de este nodo (incluido la extension).
   // a_fileClass: Clase del fichero calculado en base a al extension.
   // a_fullPath: Nombre completo (si fue solicitado)
   // a_variablePath: Nombre teniendo en cuenta la variable de entorno
   //   en la que esta hubidado por ejemplo $(MYPJ)/er.ss (si fue solicitado)
   //-----------------------------------------------------------------------
   File* a_parent;
   std::string a_name;
   const FileClass* a_fileClass;   
   std::string* a_fullPath;
   std::string* a_variablePath;
   
   static void copy (std::string** target, const std::string* source) throw ();
   
   friend class SortByName;
};

}
}

#endif

