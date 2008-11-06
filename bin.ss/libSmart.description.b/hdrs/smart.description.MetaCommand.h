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
#ifndef _smart_description_MetaCommand_h
#define _smart_description_MetaCommand_h

#include <nemesis.Recycler.h>

namespace smart {

namespace description {

class MetaLine;
class RuleAgent;
class Rule;

using namespace nemesis;

class MetaCommand {
public:
   typedef MetaLine** metaline_iterator;
   typedef MetaLine*const* const_metaline_iterator;
   
   const_metaline_iterator metaline_begin () const throw () { return a_metalines; }
   const_metaline_iterator metaline_end () const throw () { return a_metalines + a_maxLine; }
   static const MetaLine* metaline (const_metaline_iterator ii) throw () { return *ii; }
        
   std::string asString () const throw ();
        
private:
   static const int MaxLine = 5;
   const Rule* a_rule;
   int a_maxLine;
   MetaLine* a_metalines [MaxLine];
   
   MetaCommand (const Rule* rule) : a_rule (rule), a_maxLine (0) {;}
   MetaLine* createLine () throw (RuntimeException);       
   void compile () throw (RuntimeException);

   metaline_iterator metaline_begin () throw () { return a_metalines; }
   metaline_iterator metaline_end () throw () { return a_metalines + a_maxLine; }
   static MetaLine* metaline (metaline_iterator ii) throw () { return *ii; }
   
   friend class Rule;
   friend class RuleAgent;
};

}
}

#endif
