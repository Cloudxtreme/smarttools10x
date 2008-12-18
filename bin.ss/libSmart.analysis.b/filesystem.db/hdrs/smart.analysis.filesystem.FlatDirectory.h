/*
   Smart.analysis - Libreria para el analisis de depedencias entre componentes software.
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
#ifndef _smart_analysis_filesystem_FlatDirectory_h
#define _smart_analysis_filesystem_FlatDirectory_h

#include <nemesis.defines.h>
#include <nemesis.SafeSortedVector.h>

#include <smart.naming.File.h>

#include <smart.analysis.filesystem.FlatEntry.h>

namespace smart {

namespace analysis {

namespace filesystem {

class Node;
class FlatEntry;

using namespace nemesis;

class FlatDirectory {
public:
   typedef SafeSortedVector <FlatEntry, naming::File::SortByName, std::string> entry_container;
   typedef entry_container::iterator entry_iterator;
   typedef entry_container::const_iterator const_entry_iterator;
   
   FlatDirectory () {;}

   void add (Node*) throw (RuntimeException);
   
   FlatEntry* find (const naming::File* file) throw (RuntimeException) { return a_entries.find (file->getName ()); }      
   const FlatEntry* find (const naming::File* file) const throw (RuntimeException) {
      return a_entries.find (file->getName ());
   }

   entry_iterator entry_begin () throw () { return a_entries.begin (); }
   entry_iterator entry_end () throw () { return a_entries.end (); }

   const_entry_iterator entry_begin () const throw () { return a_entries.begin (); }
   const_entry_iterator entry_end () const throw () { return a_entries.end (); }

   static FlatEntry* entry (entry_iterator& ii) throw () { return entry_container::data (ii); }
   static const FlatEntry* entry (const_entry_iterator& ii) throw () { return entry_container::data (ii); }

private:
   entry_container a_entries;
   
   FlatDirectory (const FlatDirectory&);   
};

}
}
}

#endif
