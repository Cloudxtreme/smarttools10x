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
#ifndef _smart_generate_Command_h
#define _smart_generate_Command_h

#include <vector>

#include <nemesis.DataBlock.h>

#include <smart.naming.File.h>

#include <smart.description.MetaLine.h>

#include <smart.generate.Prototype.h>

namespace smart {

namespace description {
   class Rule;
}

namespace analysis {
   class CSource;
}

namespace generate {

using namespace nemesis;

class Command : public Prototype {
public:
   Command ();

private:
   typedef std::vector <const naming::File*> directory_container;
   typedef directory_container::iterator directory_iterator;

   std::string a_buffer;
   DataBlock a_includes;
   DataBlock a_dependences;
   directory_container a_includeDirectories;
   const analysis::filesystem::Node* a_predecessorNode;
        
   directory_iterator directory_begin () throw () { return a_includeDirectories.begin (); }
   directory_iterator directory_end () throw () { return a_includeDirectories.end (); }
   static const naming::File* directory (directory_iterator& ii) throw () { return *ii; }

   void initialize (Prototype*, const analysis::filesystem::Node*) throw (RuntimeException);
   void execute () throw (RuntimeException);
   void clear () throw ();      
   
   std::string writeOutput (generate::Makefile&, const description::Rule*, const naming::File& input) throw (RuntimeException);
   std::string writeFirstInput (generate::Makefile&, const analysis::CSource*) throw (RuntimeException);
   std::string writeOtherInput (generate::Makefile&, const analysis::CSource*, const naming::File& input) throw (RuntimeException);
   void writeCommand (generate::Makefile&, const description::Rule*, const analysis::CSource*, const std::string& input, const std::string& output) 
      throw (RuntimeException);   
   void setText (description::MetaLine::DynamicText::_v, const char* text) throw (RuntimeException);   
   std::string getRelativeOutput (const std::string& filename, const naming::File&) 
      throw (RuntimeException);
};

}
}

#endif

