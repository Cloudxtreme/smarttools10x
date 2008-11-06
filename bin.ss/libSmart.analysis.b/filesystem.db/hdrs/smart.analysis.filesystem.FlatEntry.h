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
#ifndef _smart_analysis_filesystem_FlatEntry_h
#define _smart_analysis_filesystem_FlatEntry_h

#include <nemesis.defines.h>

#include <smart.naming.File.h>

namespace smart {

namespace analysis {

namespace filesystem {

class FlatDirectory;
class Node;

using namespace nemesis;

//--------------------------------------------------------------------------------
// Registra todos los nombre completos con los que ha sido encontado 
// un determinado archivo de cabecera.
//--------------------------------------------------------------------------------
class FlatEntry : public naming::File {
public:
   typedef Node* FullPath;
   typedef const FullPath* const_path_iterator;
   typedef FullPath* path_iterator;

   void add (Node* node) throw (RuntimeException);

   const Node* getPath (const int pos) const throw (RuntimeException);

   int path_size () const throw () { return a_maxPath; }
   
   bool contains (const Node*) const throw ();

   path_iterator path_begin () throw () { return a_fullPaths; }
   path_iterator path_end () throw () { return a_fullPaths + a_maxPath; }

   const_path_iterator path_begin () const throw () { return a_fullPaths; }
   const_path_iterator path_end () const throw () { return a_fullPaths + a_maxPath; }

   std::string asString () const throw ();

   static FullPath path (path_iterator ii) throw () { return *ii; }
   static const FullPath path (const_path_iterator ii) throw () { return *ii; }

private:
   static const int MaxPath = 25;

   FullPath a_fullPaths [MaxPath];
   int a_maxPath;

   FlatEntry (const naming::File* file) : naming::File (*file), a_maxPath (0) {
      nemesis_memset (a_fullPaths, 0, sizeof (a_fullPaths));
   }
   
   friend class FlatDirectory;
};

}
}
}

#endif

