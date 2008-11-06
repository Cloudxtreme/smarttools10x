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
#ifndef _smart_analysis_filesystem_Node_h
#define _smart_analysis_filesystem_Node_h

#include <nemesis.SortedVector.h>

#include <smart.description.Rule.h>

#include <smart.naming.File.h>

namespace smart {

namespace analysis {

namespace filesystem {

class Path;

using namespace nemesis;

//-----------------------------------------------------------------------------------------
// successor_container: Mantiene los sucesores fisicos de una entrada del directorio.
// flatdep_container: Mantiene las dependencias logicas de una entrada.de primer nivel
//-----------------------------------------------------------------------------------------
class Node : public naming::File {
public:
   const description::Rule* getRule () const throw () { return a_rule; }
   filesystem::Path* getPath () throw () { return (Path*) getParent (); }

   bool isAPath () const throw () { return (a_rule == NULL) ? true: a_rule->isAPath (); }
   bool isAHeader () const throw () { return (a_rule == NULL) ? false: a_rule->getType () == description::Rule::Type::Header; }

   void setRule (const description::Rule* rule) throw () { a_rule = rule; }
   
   void interpreted () throw () { a_mask |= Interpreted; }
   
   bool isWritten () const throw () { return (a_mask & Written) != 0; }
   void written () throw () { a_mask |= Written; }
   
   void interpret () throw (RuntimeException);
   void synthesize () throw (RuntimeException);
   
protected:
   enum Mask { None, OnAir, Interpreted = 2, Synthesized = 4, Written = 8 };

   int a_mask;
   
   Node (const Variable* variable) : 
      naming::File (variable), 
      a_rule (NULL)
   {      
      a_mask = None;
   }
   
   Node (Node* parent, const naming::File& file, const description::Rule* rule) : 
      naming::File (parent, file.getName ()),
      a_rule (rule)            
   {
      a_mask = None;
   }

   bool isInterpreted () const throw () { return (a_mask & Interpreted) != 0; }
   
   bool isSynthesized () const throw () { return (a_mask & Synthesized) != 0; }
   void synthesized () throw () { a_mask |= Synthesized; }

private:
   const description::Rule* a_rule;
   
   Node (const Node&);
   
   bool begin () throw (RuntimeException) { return (a_mask & OnAir) ? false: ((a_mask |= OnAir) != 0); }
   void end () throw (RuntimeException) { a_mask &= ~OnAir; }
   
   virtual void do_interpret () throw (RuntimeException) = 0;
   virtual void do_synthesize () throw (RuntimeException) = 0;
};

}
}
}

#endif
