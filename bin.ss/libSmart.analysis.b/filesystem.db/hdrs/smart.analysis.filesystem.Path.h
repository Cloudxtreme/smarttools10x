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
#ifndef _smart_analysis_filesystem_Path_h
#define _smart_analysis_filesystem_Path_h

#include <smart.analysis.filesystem.Node.h>

namespace smart {

namespace description {
   class Rule;
}

namespace analysis {

namespace filesystem {

class FlatDirectory;

using namespace nemesis;

class Path : public Node {
public:
   struct ScanMode { enum _v { Full, Quick, Virtual }; };

   typedef SortedVector <Node, SortByName, const std::string&> successor_container;
   typedef successor_container::iterator successor_iterator;
   typedef successor_container::const_iterator const_successor_iterator;
   
   typedef std::vector <Node*> manualorder_container;
   typedef manualorder_container::iterator manualorder_iterator;

   Path (const Variable*, const ScanMode::_v scanMode);
   Path (Path* parent, const naming::File&, const description::Rule*, const bool manualOrder);

   void initialize () throw (RuntimeException);
   
   ScanMode::_v getScanMode () const throw () { return a_scanMode; }   
   FlatDirectory* getFlatDirectory () throw () { return a_flatDirectory; }
   const FlatDirectory* getFlatDirectory () const throw () { return a_flatDirectory; }
   bool hasManualOrder () const throw () { return a_manualorders != NULL; }

   void setScanMode (const ScanMode::_v scanMode) throw () { a_scanMode = scanMode; }

   Node* successor_find (const std::string& name) throw () { return a_successors.find (name); }
   Node* successor_find (const naming::File& file) throw () { return a_successors.find (file.getName ()); }
   successor_iterator successor_begin () throw () { return a_successors.begin (); }
   successor_iterator successor_end () throw () { return a_successors.end (); }

   const_successor_iterator successor_begin () const throw () { return a_successors.begin (); }
   const_successor_iterator successor_end () const throw () { return a_successors.end (); }

   manualorder_iterator manualorder_begin () throw () { return a_manualorders->begin (); }
   manualorder_iterator manualorder_end () throw () { return a_manualorders->end (); }
   
   Node* create (const naming::File&, const description::Rule*, const bool manualOrder = false) throw ();
   Path* seek (const char* path) throw (RuntimeException);

   static Node* successor (successor_iterator& ii) throw () { return successor_container::data (ii); }
   static const Node* successor (const_successor_iterator& ii) throw () { return successor_container::data (ii); }

   static Node* manualorder (manualorder_iterator& ii) throw () { return *ii; }

private:
   ScanMode::_v  a_scanMode;
   successor_container a_successors;
   FlatDirectory* a_flatDirectory;
   manualorder_container* a_manualorders;
   
   void do_interpret () throw (RuntimeException);
   void do_synthesize () throw (RuntimeException);
};

}
}
}

#endif

