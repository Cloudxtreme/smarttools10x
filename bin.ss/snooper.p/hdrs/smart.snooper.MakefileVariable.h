/*
   snooper - Version 10.
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
#ifndef _smart_snooper_MakefileVariable_h
#define _smart_snooper_MakefileVariable_h

#include <QListWidget>

#include <nemesis.RuntimeException.h>

namespace smart {

class Variable;

namespace snooper {

using namespace nemesis;

class MakefileVariable : public QListWidgetItem {
public:
   struct SortByName {
      static const std::string& value (const MakefileVariable* v) throw () { return v->getName (); }
   };
   
   MakefileVariable (QListWidget*, const char* name);

   const char* getValue () const throw () { return a_value.c_str (); }
   const std::string& getName () const throw () { return a_name; }

   void add (const char* value) throw () { a_value += value; }
   
   std::string asString () const throw ();
   
private:
   const std::string a_name;
   std::string a_value;
};

}
}

#endif
