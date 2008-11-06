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
#include <ctype.h>

#include <nemesis.functions.h>

#include <smart.description.MetaLine.h>

using namespace std;
using namespace nemesis;
using namespace smart;

description::MetaLine::MetaLine ()
{
   nemesis_memset (a_staticTexts, 0, sizeof (a_staticTexts));
   nemesis_memset (a_dynamicIndicators, 0, sizeof (a_dynamicIndicators));
}

void description::MetaLine::setText (const description::MetaLine::StaticText::_v v, const string& text) 
   throw ()
{
   static struct {
      const char* code;
      const char* character;
   } translate [] = { ":gt", ">", ":lt", "<", NULL, 0 };

   int pos, n;
   
   if (strip (text, pos, n) == true) {
      string* str = new string (text.substr (pos, n));
      string::size_type pos;
      
      for (int i = 0; translate [i].code != NULL; i ++) {
         while ((pos = str->find (translate [i].code)) != string::npos) 
            str->replace (pos, nemesis_strlen (translate [i].code), translate [i].character);
      }
      
      a_staticTexts [v] = str;
   }
}

void description::MetaLine::setIndicator (const description::MetaLine::DynamicText::_v v, const string& indicator) 
   throw ()
{
   a_dynamicIndicators [v] = new string (indicator);
}

//--------------------------------------------------------------------------------------------
// (1) El indicador de los includes aparecera cada vez que se indique un directorio de
// busqueda, por lo que el indicador concreto tendra que ser insertado para cada entrada
//--------------------------------------------------------------------------------------------
void description::MetaLine::compile ()
   throw (RuntimeException)
{
   DynamicText::_v dt;
   StaticText::_v st;
   
   a_pattern = "\t$(VIEWER)";   
   for (st = StaticText::Beginning, dt = DynamicText::Input; st < StaticText::End; st = next (st), dt = next (dt)) {
      if (a_staticTexts [st] != NULL) {
         a_pattern += *a_staticTexts [st];
         a_pattern += ' ';
      }   
      if (dt < DynamicText::End && a_dynamicIndicators [dt] != NULL) {
         if (multipleIndicator (dt) == false)                               // (1) 
            a_pattern += *a_dynamicIndicators [dt];
         a_pattern += DynamicText::asLiteral (dt);
         a_pattern += ' ';
      }
   }
}

//---------------------------------------------------------------------------------------
// (1) +1 porque tiene que incluir el caracter que acaba de descontar y que ya no es
// un 'isspace'
//---------------------------------------------------------------------------------------
bool description::MetaLine::strip (const std::string& text, int& pos, int& n) 
   throw ()
{
   const char* ii;
   
   for (pos = 0, ii = text.c_str (); *ii != 0 && isspace (*ii); ii ++, pos ++);

   if (*ii == 0)   
      return false;
      
   n = nemesis_strlen (ii) - 1;   
   for (const char* ww = ii + n; ww >= ii && isspace (*ww); ww --, n--);
   n ++;                                                                      // (1)   
   
   return (n > 0);
}   

const char* description::MetaLine::DynamicText::asLiteral (description::MetaLine::DynamicText::_v v) 
   throw ()
{
   static const char* text [] = { 
      "(%Input%)", "(%InputOnlyName%)", "(%Includes%)", "(%Dependences%)", "(%VDependences%)", "(%Output%)", "(%End%)" 
   }; 

   return text [v];
}
