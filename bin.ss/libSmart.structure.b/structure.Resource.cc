/*
   Smart.structure - Version 10.
   Libreria para gestionar la estructura de resource/vendor/version implicados a la hora de 
   la generacion de ejecutables. 

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

#include <smart.Variable.h>

#include <smart.structure.Resource.h>

using namespace std;
using namespace smart;

structure::Resource::Resource (const string& name, const bool onTarget, const bool enableILS, const bool isASection) :
   a_name (name), a_onTarget (onTarget), a_enableILS (enableILS),
   a_isASection (isASection),
   a_parent (NULL)
{
   string varname ("GMAKE_");
   
   const char* sname = name.c_str ();
   
   while (*sname != 0)
      varname += toupper (*sname ++);
      
   a_variable = new Variable (varname.c_str ());
}

structure::Resource::~Resource ()
{
   delete a_variable;
}

std::string structure::Resource::serialize () const 
   throw () 
{ 
   string result (a_name);
   result += functions::asText (":", (int) a_onTarget);   
   return result += functions::asText (":", (int) a_enableILS);   
}
