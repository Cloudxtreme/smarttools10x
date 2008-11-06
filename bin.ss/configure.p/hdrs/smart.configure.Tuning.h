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
#ifndef smart_configure_Tuning_h
#define smart_configure_Tuning_h

#include <string>
#include <vector>

#include <nemesis.RuntimeException.h>

namespace smart {

namespace configure {

class Application;

using namespace nemesis;

class Tuning {
public:
   typedef std::string ResourceName;
   
   class Selection {
   public:
      Selection () : a_key (-1), a_value (NULL) {;}
      Selection (const Selection& other) : a_key (other.a_key) {
         a_value = (other.a_value == NULL) ? NULL: new std::string (*other.a_value);
      }
      ~Selection () { delete a_value; }

      int getKey () const throw () { return a_key; }
      const std::string* getValue () const throw () { return a_value; } 
      
      Selection& operator= (const int key) throw () {
         a_key = key;
         delete a_value;
         a_value = NULL;
         return *this; 
      }      
      Selection& operator= (const char* value) throw () {
         delete a_value;
         a_value = new std::string (value);
         a_key = -1;
         return *this;
      }
      Selection& operator= (const Selection& other) throw () {
         if (this != &other) {
            a_key = other.a_key;
            a_value = (other.a_value == NULL) ? NULL: new std::string (*other.a_value);
         }
         return *this;
      }
      
      std::string asString () const throw ();
      
   private:
      int a_key;
      std::string* a_value;      
   };
   
   typedef std::pair <ResourceName, Selection> entry;
   typedef std::vector <entry> container;
   typedef container::iterator iterator;
   typedef container::const_iterator const_iterator;
   
   void initialize (const char* filename) throw (RuntimeException);

   const_iterator begin () const throw () { return a_container.begin (); }
   const_iterator end () const throw () { return a_container.end (); }
   const_iterator find (const ResourceName&) const throw ();

   static const Selection& selection (const_iterator& ii) throw () { return ii->second; }

private:
   container a_container;
   
   static const char* st_dtd;
   
   Tuning () {;}
   
   static const ResourceName& resource_name (const_iterator& ii) throw () { return ii->first; }
   
   friend class Application;   
};

}
}

#endif
