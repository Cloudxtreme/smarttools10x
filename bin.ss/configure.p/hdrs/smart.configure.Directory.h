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
#ifndef _smart_configure_Directory_h_
#define _smart_configure_Directory_h_

#include <vector>

#include <nemesis.io.Directory.h>

namespace smart {

namespace configure {

using namespace nemesis;

class Directory : public io::Directory {
public:
   struct Mode { enum _v { Normal, NoVendor }; };

   typedef Directory Predecessor;
   typedef Directory Successor;

   typedef std::vector <Successor*> successor_container;
   typedef successor_container::iterator successor_iterator;
   typedef successor_container::const_iterator const_successor_iterator;
   
   virtual ~Directory ();

   const std::string& getId () const throw () { return a_id; }
   const std::string& getTitle () const throw () { return a_title; }

   void setTitle (const std::string& title) throw () { a_title = title; }
   
   void add (Successor* successor) throw () {
      a_successors.insert (successor_begin (), successor);        
      successor->a_predecessor = this;
   }   
   bool isNone () const throw () { return a_id == "none" || a_id == "none.include"; }

   successor_iterator successor_begin () throw () { return a_successors.begin (); }
   successor_iterator successor_end () throw () { return a_successors.end (); }
   int successor_size () const throw () { return a_successors.size (); }

   const_successor_iterator successor_begin () const throw () { return a_successors.begin (); }
   const_successor_iterator successor_end () const throw () { return a_successors.end (); }
   const Successor* successor_find (const std::string& id) const throw ();

   static Successor* successor (successor_iterator& ii) throw () { return *ii; }
   static const Successor* successor (const_successor_iterator& ii) throw () { return *ii; }

   virtual void initialize (const std::string& path) throw (RuntimeException) = 0;

protected:
   std::string a_title;
   
   Directory (const std::string& id) : a_id (id), a_predecessor (NULL) {;}
   
   const Predecessor* getPredecessor () const throw () { return a_predecessor; }

private:
   const std::string a_id;
   const Predecessor* a_predecessor;
   successor_container a_successors;   
};

}
}

#endif

