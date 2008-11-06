/*
   Smart.description - Version 10.
   Libreria para la descripcion de los elementos software con los que vamos a trabajar.

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
#include <nemesis.defines.h>

#include <nemesis.xml.Node.h>
#include <nemesis.xml.Attribute.h>

#include <smart.description.functions.h>

using namespace std;
using namespace nemesis;
using namespace smart;

string description::functions::st_targetDirectory;
string description::functions::st_targetPrefix;
string description::functions::st_targetName;
string description::functions::st_targetExtension;
string description::functions::st_makeName;
string description::functions::st_makeConfig;
string description::functions::st_makeResource;
string description::functions::st_version;
string description::functions::st_projectConfig;
string description::functions::st_tuneProjectConfig;

void description::functions::setup (const xml::Node* root)
   throw (RuntimeException)
{
   const xml::Node* node = root->find ("TargetDirectory");
   
   st_targetExtension = node->getAttribute ("Extension")->getValue ();
   
   const char* extension = st_targetExtension.c_str ();
   
   if (nemesis_strchr (extension, '.') == NULL) 
      throw RuntimeException (
         "La variable XML TargetDirectory::Extension debe contener un '.'", FILE_LOCATION
      );   
      
   const int len = nemesis_strlen (extension);
   
   if (len == 0) 
      throw RuntimeException (
         "La variable XML TargetDirectory::Extension no es valida", FILE_LOCATION
      );   

   if (extension [len - 1] == '.')
      throw RuntimeException (
         "La variable XML TargetDirectory::Extension no puede terminar en '.'", FILE_LOCATION
      );   
   
   st_targetDirectory = st_targetPrefix = node->getAttribute ("Prefix")->getValue ();
   st_targetDirectory += "$(";
   st_targetDirectory += (st_targetName = node->getAttribute ("Name")->getValue ());
   st_targetDirectory += ')';
   st_targetDirectory += st_targetExtension;
   
   st_version = root->getAttribute ("Version")->getValue ();   

   node = root->find ("Project");
   st_projectConfig = node->getAttribute ("Configure")->getValue ();
   st_tuneProjectConfig = node->getAttribute ("Tune")->getValue ();
   
   node = root->find ("Makefile");
   st_makeName = node->getAttribute ("Name")->getValue ();
   st_makeConfig = node->getAttribute ("Configure")->getValue ();
   st_makeResource = node->getAttribute ("Resource")->getValue ();
}

