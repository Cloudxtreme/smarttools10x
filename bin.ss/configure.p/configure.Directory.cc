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
#include <smart.configure.Directory.h>

using namespace std;
using namespace smart;

configure::Directory::~Directory ()
{
   for (successor_iterator ii = successor_begin (), maxii = successor_end (); ii != maxii; ii ++)
      delete successor (ii);
}

const configure::Directory::Successor* configure::Directory::successor_find (const std::string& id) const 
   throw ()
{
   const_successor_iterator ii, maxii;

   for (ii = successor_begin (), maxii = successor_end (); ii != maxii; ii ++) 
      if (successor (ii)->getId () == id)
         break;
   
   return (ii != maxii) ? successor (ii): NULL;
}


