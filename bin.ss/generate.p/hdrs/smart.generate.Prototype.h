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
#ifndef _smart_generate_Prototype_h
#define _smart_generate_Prototype_h

#include <vector>

#include <nemesis.RuntimeException.h>

namespace smart {

namespace analysis {
   namespace filesystem {
      class Node;
   }
}
   
namespace generate {

class Makefile;
class Agent;

using namespace nemesis;

class Prototype {
public:
   struct Type {
      enum _v { 
         FileSystem, Executable, Library, LogicalGroup, Command
      };
   };
         
   Type::_v getType () const throw () { return a_type; }
   const analysis::filesystem::Node* getNode () const throw () { return a_node; }
   
   virtual void execute () throw (RuntimeException) = 0;
   
protected:
   typedef std::vector <const analysis::filesystem::Node*> dependence_container;
   typedef dependence_container::iterator dependence_iterator;
   typedef dependence_container::const_iterator const_dependence_iterator;
   
   const analysis::filesystem::Node* a_node;
   Prototype* a_predecessor;
         
   Prototype (const Type::_v type) : a_type (type), a_node (NULL) {;}
   
   virtual void initialize (Prototype* predecessor, const analysis::filesystem::Node* node) 
      throw (RuntimeException) 
   {
      a_node = node;
      a_predecessor = predecessor;
   }
   virtual void clear () throw () { a_node = NULL; }
   virtual void copy (const Prototype* other) throw () {;}

   static const analysis::filesystem::Node* dependence (dependence_iterator& ii) throw () { return *ii; }
   static const analysis::filesystem::Node* dependence (const_dependence_iterator& ii) throw () { return *ii; }
   
private:
   const Type::_v a_type;
      
   friend class Agent;
};

}
}

#endif

