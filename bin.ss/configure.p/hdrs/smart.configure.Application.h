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
#ifndef _smart_configure_Application_h
#define _smart_configure_Application_h

#include <nemesis.app.Application.h>

#include <smart.configure.Outline.h>

namespace smart {

namespace configure {

class Resource;
class Tuning;

using namespace nemesis;

class Application : public app::Application {
public:
   Application ();
   
   const Tuning* getTuning () const throw () { return a_tuning; }
   
   void write (const Resource* resource, const std::string& variable, const std::string& value)
      throw (RuntimeException);
   
private:
   Outline a_outline;
   int a_ostream;
   std::string a_target;
   std::string a_baseTarget;
   Tuning* a_tuning;

   void initialize () throw (RuntimeException);
   void run () throw (RuntimeException);      
   
   bool load (Outline&, const std::string& path) throw (RuntimeException);
};

}
}

#endif
