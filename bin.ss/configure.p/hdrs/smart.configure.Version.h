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
#ifndef smart_configure_Version_h
#define smart_configure_Version_h

#include <smart.configure.Directory.h>

namespace smart {

namespace configure {

class Vendor;
class Resource;

class Version : public Directory {
public:
   Version (const std::string& id, const Mode::_v mode) : Directory (id), a_mode (mode) {;}
      
   const std::string& getValue () const throw () { return a_value; }
   
   const Resource* getResource() const throw ();
   const Vendor* getVendor () const throw () { return (const Vendor*) getPredecessor (); }
      
   void initialize (const std::string& path) throw (RuntimeException);
   void paint (const int noption) const throw ();
   void set () const throw ();
            
   static std::string normalizeName (const std::string& entry) throw ();
   
private:
   const Mode::_v a_mode;
   std::string a_value;
};

}
}

#endif
