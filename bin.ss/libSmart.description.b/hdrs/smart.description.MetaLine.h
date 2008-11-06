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
#ifndef _smart_description_MetaLine_h
#define _smart_description_MetaLine_h

#include <nemesis.RuntimeException.h>

namespace smart {

namespace description {

class MetaCommand;

using namespace nemesis;

class MetaLine {
public:
   struct StaticText { 
      enum _v { Beginning = 0, PostInput, PostInputOnlyName, PostIncludes, PostDependences, PostVDependences, PostOutput, End }; 
   };
   struct DynamicText { 
      enum _v { Input, InputOnlyName, Includes, Dependences, VDependences, Output, End }; 
      static const char* asLiteral (_v v) throw ();
   };
   
   const char* getPattern () const throw () { return a_pattern.c_str (); }
   const char* getIndicator (const DynamicText::_v v) const throw () { 
      return (a_dynamicIndicators [v] == NULL) ? NULL: a_dynamicIndicators [v]->c_str ();
   }
   
   void setText (const StaticText::_v v, const std::string& text) throw ();
   void setIndicator (const DynamicText::_v v, const std::string& indicator) throw ();
   
   bool hasDynamicText (const DynamicText::_v v) const throw () { return a_dynamicIndicators [v] != NULL; }
   
   static DynamicText::_v next (DynamicText::_v v) throw () { return (DynamicText::_v) (((int) v) + 1); }
   static StaticText::_v next (StaticText::_v v) throw () { return (StaticText::_v) (((int) v) + 1); }
   static bool multipleIndicator (const DynamicText::_v v) throw () {
      return v >= DynamicText::Includes && v <= DynamicText::VDependences;
   }
   
private:
   std::string* a_staticTexts [StaticText::End];
   std::string* a_dynamicIndicators [DynamicText::End];
   std::string a_pattern;
   
   MetaLine ();
   void compile () throw (RuntimeException);   
   static bool strip (const std::string&, int& pos, int& n) throw ();
   
   
   friend class MetaCommand;
};

}
}

#endif
