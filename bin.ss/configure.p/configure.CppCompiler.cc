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
#include <smart.configure.CppCompiler.h>
#include <smart.configure.Vendor.h>
#include <smart.configure.Version.h>

using namespace std;
using namespace nemesis;
using namespace smart;

const configure::Version* configure::CppCompiler::configure () const 
   throw (RuntimeException)
{
   const Version* version = Resource::configure ();
   
   string path (a_path);
   path += '/';
   path += version->getVendor ()->getId ();
   path += '/';
   path += version->getId ();
   
   CppCompiler* _this = const_cast <CppCompiler*> (this);   
   _this->a_outline.initialize (path);
   _this->a_outline.run ();   
   
   return version;
}

