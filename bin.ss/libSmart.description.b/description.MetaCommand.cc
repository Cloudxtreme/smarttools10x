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
#include <nemesis.functions.h>
#include <nemesis.Logger.h>

#include <smart.description.MetaCommand.h>
#include <smart.description.MetaLine.h>
#include <smart.description.Rule.h>

using namespace std;
using namespace nemesis;
using namespace smart;

description::MetaLine* description::MetaCommand::createLine () 
   throw (RuntimeException)
{
   if (a_maxLine == MaxLine) {
      string msg ("smart::description::MetaCommand::createLine  | ");
      msg += a_rule->asString ();
      msg += functions::asText (" | Sobrepasada el numero maximo de ocurrencias (", MaxLine);
      msg += ')';
      throw RuntimeException (msg, FILE_LOCATION);
   }

   return a_metalines [a_maxLine ++] = new MetaLine ();
}

void description::MetaCommand::compile () 
   throw (RuntimeException)
{   
   for (metaline_iterator ii = metaline_begin (), maxii = metaline_end (); ii != maxii; ii ++) 
      metaline (ii)->compile ();  
      
   LOGINFORMATION (
      string msg ("description::MetaCommand::compile | ");
      msg += a_rule->asString ();
      msg += " | ";
      msg += asString ();
      Logger::information (msg, FILE_LOCATION);
   );
}

string description::MetaCommand::asString () const
   throw ()
{
   string result ("smart:: description::MetaCommand { ");
   
   for (const_metaline_iterator ii = metaline_begin (), maxii = metaline_end (); ii != maxii; ii ++) {
      result += "\n Line {";
      result += metaline (ii)->getPattern ();
      result += "}";
   }
   
   return result += " }";   
}

   
