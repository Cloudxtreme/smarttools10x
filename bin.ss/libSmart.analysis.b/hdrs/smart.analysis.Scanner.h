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
#ifndef _smart_analysis_Scanner_h
#define _smart_analysis_Scanner_h

#include <vector>

#include <nemesis.Runnable.h>
#include <nemesis.Thread.h>

namespace nemesis {
   namespace io {
      class TextReader;
   }
}

namespace smart {

class Variable;

namespace description {
   class RuleAgent;
   class Rule;
}

namespace analysis {

class FileSystem;

namespace filesystem {
   class Path;
}

using namespace nemesis;

class Scanner : public Runnable {
public:
   Scanner (FileSystem*, const Variable*);
   
   filesystem::Path* getPath () throw () { return a_path; }
   const filesystem::Path* getPath () const throw () { return a_path; }
   
   void execute () throw ();
   void verify () const throw (RuntimeException);
   
   static void setRuleAgent (const description::RuleAgent& ruleAgent) throw () {
      st_ruleAgent = &ruleAgent;
   }
   
private:
   FileSystem& a_fileSystem;   
   Thread a_thread;
   filesystem::Path* a_path;
   RuntimeException* a_exception;   
   
   typedef std::vector <std::string> order_container;
   typedef order_container::iterator order_iterator;
   
   static const description::RuleAgent* st_ruleAgent;
   
   void initialize () throw (RuntimeException);
   void run () throw (RuntimeException);
   void terminate () throw ();
   void do_action () throw () {;}
   
   static void forward (filesystem::Path*, std::string& aux) throw (RuntimeException);
   static void analize (filesystem::Path*, naming::File&, std::string& aux, const bool manualOrder) 
      throw (RuntimeException);
   static void preload (io::TextReader&, order_container&, std::string& aux) throw (RuntimeException);
   
   static void expand (const description::Rule*, filesystem::Node*, naming::File&) throw (RuntimeException);
};

}
}

#endif
