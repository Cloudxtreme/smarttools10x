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
   
#include <iostream>

#include <nemesis.functions.h>

#include <smart.naming.File.h>
#include <smart.naming.FileClass.h>

#include <smart.Variable.h>

using namespace std;
using namespace nemesis;
using namespace smart;

naming::File::File () :
   a_parent (NULL)
{
   a_fileClass = NULL;   
   a_fullPath = NULL;
   a_variablePath = NULL;
}

naming::File::File (const Variable* variable) :
   a_parent (NULL),
   a_fileClass (NULL),
   a_fullPath (NULL)
{
   try {
      const string& value = variable->getValue ();
   
      string::size_type pos = value.find_last_of ('/');

      if (pos == string::npos)
         a_name = value;
      else if (pos == (value.length () - 1)) 
         a_name = value.substr (0, pos);
      else
         a_name = value;
      
      a_fullPath = new string (a_name);
      a_fileClass = FileClass::instance (*this);     
   }
   catch (RuntimeException& ex) {
      ex.trace ();
   }
   
   a_variablePath = new string ("$(");
   *a_variablePath += variable->getName ();
   *a_variablePath += ')';
}

naming::File::File (const File& other) 
{
   a_fullPath = NULL;
   a_variablePath = NULL;   
   operator=(other);
}

naming::File::File (naming::File* parent, const string& name) :
   a_parent (parent),
   a_name (name)
{
   a_fileClass = FileClass::instance (*this);
   a_fullPath = NULL;
   a_variablePath = NULL;
}

naming::File::File (naming::File* parent, const char* name) :
   a_parent (parent),
   a_name (name)
{
   a_fileClass = FileClass::instance (*this);
   a_fullPath = NULL;
   a_variablePath = NULL;
}

naming::File::File (naming::File* parent) :
   a_parent (parent)
{   
   a_fileClass = NULL;
   a_fullPath = NULL;
   a_variablePath = NULL;
}

naming::File::~File ()
{
   delete a_fullPath;     
   delete a_variablePath;   
}

void naming::File::setName (const char* name)
   throw ()
{
   a_name = name;
   a_fileClass = FileClass::instance (*this);
   
   delete a_fullPath;
   a_fullPath = NULL;
   
   delete a_variablePath;
   a_variablePath = NULL;
}

void naming::File::setName (const char* name, const char* ext) 
   throw ()
{
   a_name = name;
   a_name += '.';
   a_name += ext;
   a_fileClass = FileClass::instance (*this);
   
   delete a_fullPath;
   a_fullPath = NULL;
   
   delete a_variablePath;
   a_variablePath = NULL;
}

const naming::FileClass* naming::File::replaceName (const std::string& name) 
   throw ()
{
   bool deleteFullPath = true;

   a_name = name;

   if ((a_fileClass = FileClass::instance (*this)) != NULL) {
      if (a_fullPath != NULL && a_parent != NULL) {
         *a_fullPath = a_parent->getFullPath ();
         *a_fullPath += '/';
         *a_fullPath += name;
         deleteFullPath = false;
      }
   }

   if (deleteFullPath == true) {
      delete a_fullPath;
      a_fullPath = NULL;
   }
   
   delete a_variablePath;
   a_variablePath = NULL;
   
   return a_fileClass;
}

const string& naming::File::getFullPath () const 
   throw ()
{
   if (a_parent != NULL) {
      Guard guard (this, "smart::naming::File from getFullPath");

      if (a_fullPath == NULL) {
         File* _this = const_cast <File*> (this);
         _this->a_fullPath = new string (a_parent->getFullPath ());
         *(_this->a_fullPath) += '/';
         *(_this->a_fullPath) += a_name;
      }   
   }

   return *a_fullPath;
}

const string& naming::File::getVariablePath () const 
   throw ()
{
   if (a_parent != NULL) {
      Guard guard (this, "smart::naming::File from getVariablePath");

      if (a_variablePath == NULL) {
         File* _this = const_cast <File*> (this);
         _this->a_variablePath = new string (a_parent->getVariablePath ());
         *(_this->a_variablePath) += '/';
         *(_this->a_variablePath) += a_name;
      }   
   }

   return *a_variablePath;   
}

//-----------------------------------------------------------------------------
// Obtiene el nombre relativo de este nodo respecto a un supuesto predecesor
// Si el nodo recibido no es predecesor de este nodo => se devuelve el 
// nombre respecto a la variable de entorno con el que fue definido este
// nodo.
//-----------------------------------------------------------------------------
string naming::File::getRelativePath (const File* from) const 
   throw ()
{
   string result (a_name);
   const File* ii (NULL);
   
   for (ii = a_parent; ii != from && ii != NULL; ii = ii->a_parent) {
      result.insert (0, "/");
      result.insert (0, ii->a_name);
   }

   return (ii != NULL) ? result: getVariablePath ();
}

naming::File& naming::File::operator= (const naming::File& other) 
   throw ()
{
   if (this != &other) {
      a_parent = other.a_parent;
      a_name = other.a_name;
      a_fileClass = a_fileClass;

      copy (&a_fullPath, other.a_fullPath);
      copy (&a_variablePath, other.a_variablePath);
   }
   
   return *this;
}

std::string naming::File::getNameWithoutExtension () const 
   throw ()
{
   string::size_type pos = a_name.find_last_of ('.');      
   return (pos != string::npos) ? a_name.substr (0, pos): a_name;
}

string naming::File::asString () const
   throw ()
{
   string result ("smart::naming::File { Parent: ");
   result += functions::asHexString (nemesis_ptrnumber_cast (a_parent));
   result += functions::asHexText (" | This: ",  nemesis_ptrnumber_cast (this));
   result += " | Name: ";
   result += a_name;
   result += " | VPath: ";
   result += getVariablePath ();
   return result += " }";
}

void  naming::File::copy (string** target, const string* source)
   throw ()
{
/*
   if (*target == NULL && source == NULL) {
      none;
   }
*/   
   if (*target == NULL && source != NULL) {
      *target = new string (*source);
   }
   else if (*target != NULL && source == NULL) {
      delete *target;
      *target = NULL;
   }
   else if (*target != NULL && source != NULL) {
      **target = *source;
   }
}

