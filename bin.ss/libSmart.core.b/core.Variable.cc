/*
   Smart.core - Core de las aplicaciones Smart. 
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
   
#include <iostream>

#include <stdlib.h>

#include <nemesis.functions.h>
#include <nemesis.Logger.h>

#include <smart.Variable.h>

using namespace std;
using namespace nemesis;

bool smart::Variable::st_verbose = true;

const string& smart::Variable::getValue () const
   throw (RuntimeException)
{
   if (a_value.empty () == true) {
      const char* value;

      if (st_verbose == true)
         cout << "Cargando variable " << getName () << ": " << flush;
      
      if ((value = getenv (getName ())) == NULL) {
         string msg (asString ());
         msg += " | Variable de entorno no establecida";
         throw RuntimeException (msg, FILE_LOCATION);
      }

      if (st_verbose == true)
         cout << value << endl; 

      LOGINFORMATION (
         string msg ("smart::Variable::getValue | Variable: ");
         msg += getName ();
         msg += " | Value: ";
         msg += value;
         Logger::information (msg, FILE_LOCATION);
      );

      const_cast <smart::Variable*> (this)->a_value = value;
   }
   
   return a_value;
}

string smart::Variable::asString () const 
   throw ()
{
   string result ("smart::Variable { ");
   result += nemesis::Variable::asString ();
   result += functions::asText (" | Enabled: ", a_isEnabled);
   return result += " }";
}

