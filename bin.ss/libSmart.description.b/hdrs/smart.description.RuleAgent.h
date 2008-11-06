/*
   Smart.description - Version 10.
   Libreria para la descripcion de los elementos software con los que vamos a trabajar.

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
#ifndef _smart_description_RuleAgent_h_
#define _smart_description_RuleAgent_h_

#include <vector>

#include <nemesis.Singleton.h>

#include <smart.description.Rule.h>
//#include <smart.description.MetaLine.h>

namespace nemesis {
   class Tokenizer;
   
   namespace xml {
      class Document;
      class Node;
   }
}

namespace smart {

namespace naming {
   class File;
}

namespace description {

class MetaLine;
class Rule;

using namespace nemesis;

class RuleAgent : public Singleton <RuleAgent> {
public:
   typedef std::vector<Rule*> container;
   typedef container::iterator iterator;
   typedef container::const_iterator const_iterator;
  
   void config (const xml::Document&) throw (RuntimeException);

   bool recognizeHeaderRepository (const naming::File&) const throw ();
   
   const Rule* getRuleHeaderRepository () const throw () { return a_ruleHeaderRepository; }
   
   const Rule* find (const naming::FileClass*) const throw ();

   const_iterator begin () const throw () { return a_rules.begin (); }
   const_iterator end () const throw () { return a_rules.end (); }
   static const Rule* rule (const_iterator& ii) throw () { return *ii; }   
   
private:
   static const int MaxPhase = 5;
   static const char* st_dtd;
   
   container a_rules;   
   const Rule* a_cache;
   const Rule* a_ruleHeaderRepository;
   
   RuleAgent ();

   Rule* create (const std::string& name, const Rule::Type::_v) throw (RuntimeException);
   
   iterator begin () throw () { return a_rules.begin (); }
   iterator end () throw () { return a_rules.end (); }
   static Rule* rule (iterator& ii) throw () { return *ii; }

   static void append (description::Rule* rule, const xml::Node* ww, Tokenizer& tt) 
      throw (RuntimeException);

   friend class Singleton <RuleAgent>;
};

}

}

#endif
