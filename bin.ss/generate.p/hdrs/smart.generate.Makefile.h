/*
   generate - Version 10.
   Herramienta que crea los archivos de dependencias necesarios para crear de forma optima los ejecutables.

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
#ifndef _smart_generate_Makefile_h
#define _smart_generate_Makefile_h

#include <nemesis.DataBlock.h>

#include <nemesis.SafeRecycler.h>

namespace smart {

namespace analysis {
   namespace filesystem {
      class Path;
   }
}

namespace generate {

using namespace nemesis;

class Makefile {
public:
   struct Section { enum _v { Beginning, Body, Ending }; };

   static Makefile* create (const analysis::filesystem::Path*) throw (RuntimeException);
   static void release (Makefile*) throw ();
   
   const analysis::filesystem::Path* getPath () const throw () { return a_path; }

   void activateSection (const Section::_v v) throw () { a_activeSection = a_sections [v]; }

   void write (const char* line) throw (RuntimeException) {
      a_activeSection->append (line, nemesis_strlen (line));
   }
   void writenl (const char* line) throw (RuntimeException) {
      a_activeSection->append (line, nemesis_strlen (line));
      *a_activeSection += '\n';
   }
   void write (const char c) throw (RuntimeException) { *a_activeSection += c; }
   void write (const std::string& line) throw (RuntimeException) { write (line.c_str ()); }
   void writenl (const std::string& line) throw (RuntimeException) { 
      write (line.c_str ()); 
      *a_activeSection += '\n';
   }
   void space () throw () { *a_activeSection += ' '; }
   void newline () throw () { *a_activeSection += '\n'; }
   void tab (const int maxn = 1) throw ()  { for (int n = 0; n < maxn; n ++) *a_activeSection += '\t'; }
   void debug (const char* file, const int line) throw (RuntimeException);

private:
   const analysis::filesystem::Path* a_path;
   std::string a_fullpath;
   DataBlock* a_sections [Section::Ending + 1];
   DataBlock* a_activeSection;
   int a_fd;

   static SafeRecycler<Makefile> st_pool;

   Makefile ();   
   Makefile (const Makefile&);

   void write (const DataBlock*) const throw (RuntimeException);
   void clear () throw ();   

   friend class Allocator<Makefile>;
};

}
}

#endif

