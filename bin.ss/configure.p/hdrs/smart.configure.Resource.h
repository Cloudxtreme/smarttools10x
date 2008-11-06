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
#ifndef smart_configure_Resource_h
#define smart_configure_Resource_h

#include <vector>

#include <smart.configure.Directory.h>

namespace smart {

namespace configure {

class Version;
class Outline;

using namespace nemesis;

class Resource : public Directory {
public:
   Resource (const std::string& id, const Mode::_v mode, const bool onTarget, const bool enableILS) : 
      Directory (id), 
      a_mode (mode),
      a_onTarget (onTarget),
      a_enableILS (enableILS)
   {;}
   
   const std::string& getName () const throw () { return a_name; }
   bool onTarget () const throw () { return a_onTarget; }
   bool enableILS () const throw () { return a_enableILS; }
   
   virtual void initialize (const std::string& path) throw (RuntimeException);
   virtual const Version* configure () const throw (RuntimeException);
   
   virtual std::string serialize () const throw ();
   
private:
   typedef std::vector <const Version*> option_container;
   typedef option_container::const_iterator const_option_iterator;
   
   const bool a_onTarget;
   const bool a_enableILS;
   const Mode::_v a_mode;
   std::string a_name;
   option_container a_options;

   void paint () const throw ();
   const Version* configureFromTuning (const int maxopt) const throw (RuntimeException);

   const_option_iterator option_begin () const throw () { return a_options.begin (); }   
   const_option_iterator option_end () const throw () { return a_options.end (); }   
   static const Version* option (const_option_iterator& ii) throw () { return *ii; }
};

}
}

#endif
