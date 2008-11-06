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
#include <nemesis.Tokenizer.h>
#include <nemesis.Logger.h>

#include <smart.Variable.h>

#include <smart.structure.Agent.h>
#include <smart.structure.Section.h>

using namespace std;
using namespace nemesis;
using namespace smart;

//------------------------------------------------------------------------------------
// Analiza el contenido de la variable GMAKE_STRUCTURE que puede tener un aspecto
// como:
// config {cppcompiler {release:1:0 threading:1:0 rwtools:0:0 library:0:0} xml:0:0 ssl:0:0 asn1:0:0 rdbms:1:1 ss7:1:1}
//------------------------------------------------------------------------------------
const structure::Section* structure::Agent::uploadEnvironment () 
   throw (RuntimeException)
{
   if (a_root != NULL)
      return a_root;

   Variable variable ("GMAKE_STRUCTURE");
   Tokenizer sections;
   Tokenizer resources;
   string sectionNames [2];
   Section* compiler;

   sections.activateStrip (true);
   resources.activateStrip (true);

   sections.apply (variable.getValue (), "{");
   resources.apply (sections.last (), "}");

   a_root = new Section (sections [0]);
   a_root->add (compiler = new Section (sections [1]));
   
   forward (*compiler, resources [0]);
   forward (*a_root, resources [1]);
   
   return a_root;   
}

void structure::Agent::forward (structure::Section& section, const std::string& serialResources) 
   throw (RuntimeException)
{
   enum { Name, OnTarget, EnableILS };

   Tokenizer resources;
   Tokenizer data;
   Tokenizer::const_iterator  ii, maxii;
   Resource* resource;
   
   resources.activateStrip (true);
   resources.apply (serialResources, " ");
   
   LOGINFORMATION (
      string msg ("structure::Agent::forward | ");
      msg += section.getName ();
      Logger::information (msg, FILE_LOCATION);
   );
   
   for (ii = resources.begin (), maxii = resources.end (); ii != maxii; ii ++) {
      data.apply (Tokenizer::data (ii), ":");

      const std::string& name = data [Name];
      const std::string& onTarget = data [OnTarget];
      const std::string& enableILS = data [EnableILS];
      
      resource = new Resource (
         name, functions::asBool (onTarget.c_str ()), functions::asBool (enableILS.c_str ())
      );
            
      section.add (resource);      
   }
}
