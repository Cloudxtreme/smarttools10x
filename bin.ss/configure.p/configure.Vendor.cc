/*
   configure - Version 10.
   Herramienta que establece las variables de entorno en base a la configuracion establecida por el usuario
   mediante la seleccion por los menus. Los menus son creados dinamicamente en base ala informacion contenida
   en el directorio de configuraciones.

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
#include <nemesis.Logger.h>
#include <nemesis.Tokenizer.h>

#include <smart.configure.Vendor.h>
#include <smart.configure.Version.h>

using namespace std;
using namespace nemesis;
using namespace smart;

void configure::Vendor::initialize (const std::string& path)
   throw (RuntimeException)
{     
   Version* version;

   LOGINFORMATION (
      string msg ("smart::configure::Vendor::initialize | Id: ");
      msg += getId ();
      Logger::information (msg, FILE_LOCATION);
   );   
 
   if (isNone () == true) {
      add (version = new Version ("Desactivar el uso de este recurso", Mode::Normal));
      version->initialize (path);
      return;
   }
 
   read (path, io::Directory::Mode::ShortPath);
   
   for (io::Directory::const_iterator ii = begin (), maxii = end (); ii != maxii; ii ++) {
      const std::string& entry = io::Directory::data (ii);
                            
      add (version = new Version (Version::normalizeName (entry), Mode::Normal));
      
      version->initialize (entry);
   }   
}

