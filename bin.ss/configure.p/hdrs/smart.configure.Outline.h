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
#ifndef smart_configure_Outline_h
#define smart_configure_Outline_h

#include <nemesis.Recycler.h>

#include <smart.configure.Directory.h>

namespace smart {

namespace configure {

using namespace nemesis;

class Outline : public Directory {
   struct Data {
      std::string id;
      std::string title;
      bool onTarget;
      bool enableILS;
   };
   
   typedef Recycler<Data> data_container;
   
public:
   typedef data_container::const_iterator const_data_iterator;
   
   Outline (const Mode::_v mode) : Directory ("outline"), a_mode (mode) {;}

   void initialize (const std::string& path) throw (RuntimeException);
   void run () const throw (RuntimeException);
   std::string serialize () const throw ();
   
private:
   const Mode::_v a_mode;
   data_container a_datas;      

   void paint () const throw ();
   void select () const throw ();   
   void load (const std::string& path) throw (RuntimeException);
   
   void add (const std::string& id, const std::string& title, const bool onTarget, const bool enableILS) throw () {
      Data* data = a_datas.create ();
      data->id = id;
      data->title = title;
      data->onTarget = onTarget;
      data->enableILS = enableILS;
   }
   
   const_data_iterator data_begin () const throw () { return a_datas.begin (); }
   const_data_iterator data_end () const throw () { return a_datas.end (); }   
   const_data_iterator data_find (const std::string& id) const throw ();
   bool data_contains (const std::string& id) const throw () { return data_find (id) != data_end (); }
   
   static const std::string& id (const_data_iterator& ii) throw () { return data_container::data (ii)->id; }
   static const std::string& title (const_data_iterator& ii) throw () { return data_container::data (ii)->title; }   
   static bool onTarget (const_data_iterator& ii) throw () { return data_container::data (ii)->onTarget; }   
   static bool enableILS (const_data_iterator& ii) throw () { return data_container::data (ii)->enableILS; }   
};

}
}

#endif
