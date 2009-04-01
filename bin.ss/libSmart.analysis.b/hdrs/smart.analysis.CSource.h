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
#ifndef _smart_analysis_filesystem_CSource_h
#define _smart_analysis_filesystem_CSource_h

#include <smart.analysis.filesystem.Node.h>

namespace smart {

namespace analysis {

namespace filesystem {
class Path;
}

using namespace nemesis;

class CSource : public filesystem::Node {
   struct SortByPointer {
      static ptrnumber value (const Node* node) throw () { return nemesis_ptrnumber_cast (node); }
   };

   typedef SortedVector <Node, SortByPointer, ptrnumber> firststep_container;
   typedef firststep_container::iterator firststep_iterator;

   typedef SortedVector <Node, SortByPointer, ptrnumber> include_container;
   typedef firststep_container::iterator include_iterator;

public:
   typedef firststep_container::const_iterator const_include_iterator;
   
   bool include_contains (const Node* node) const throw () { return a_includes.contains (node); }
   void firststep_add (Node* node) throw () { a_firststeps.add (node); }

   const_include_iterator include_begin () const throw () { return a_includes.begin (); }
   const_include_iterator include_end () const throw () { return a_includes.end (); }
   static const Node* include (const_include_iterator& ii) throw () { return include_container::data (ii); }

   virtual bool extractFile (char* line, naming::File&) throw ();

   Node* search (filesystem::Path* parent, const naming::File&) throw (RuntimeException);

protected:
   CSource (filesystem::Path* parent, const naming::File&, const description::Rule*);
   
   void setSingleCommentIndicator (const char* singleCommentIndicator) throw () {
      a_singleCommentIndicator = singleCommentIndicator;
   }   
   virtual bool specialLine (char* line) throw () { return false; }
   
private:  
   bool a_onComment;
   firststep_container a_firststeps;
   include_container a_includes;
   const char* a_singleCommentIndicator;

   void do_interpret () throw (RuntimeException);
   void do_synthesize () throw (RuntimeException);
   bool getFilename (char* line, naming::File& file) throw ();

   firststep_iterator firststep_begin () throw () { return a_firststeps.begin (); }
   firststep_iterator firststep_end () throw () { return a_firststeps.end (); }
   static Node* firststep (firststep_iterator& ii) throw () { return firststep_container::data (ii); }

   include_iterator include_begin () throw () { return a_includes.begin (); }
   include_iterator include_end () throw () { return a_includes.end (); }
   static Node* include (include_iterator& ii) throw () { return include_container::data (ii); }
   
   static bool complexComment (const bool onComment, char* line) throw ();
   static Node* forward (filesystem::Path*, const naming::File&) throw (RuntimeException);
   
   typedef std::pair <analysis::filesystem::Node*,analysis::filesystem::Node*> loop_search_result;   
   static loop_search_result loop_search (analysis::filesystem::Path* parent, const naming::File& file) throw (RuntimeException);   
   static analysis::filesystem::Node* result_found (loop_search_result& ii) throw () { return ii.first; }
   static analysis::filesystem::Node* result_done (loop_search_result& ii) throw () { return ii.second; }
         
   friend class filesystem::Path;
};

}
}

#endif
