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
#include <nemesis.Logger.h>
#include <nemesis.functions.h>

#include <smart.analysis.filesystem.FlatDirectory.h>
#include <smart.analysis.filesystem.FlatEntry.h>
#include <smart.analysis.filesystem.Node.h>

using namespace std;
using namespace nemesis;
using namespace smart;
using namespace smart::analysis;

void filesystem::FlatDirectory::add (filesystem::Node* node) 
   throw (RuntimeException)
{
   Guard guard (a_entries, "node_container from analysis::filesystem::FlatDirectory::add");
   
   LOGINFORMATION (
      string msg ("smart::analysis::filesystem::FlatDirectory::add | ");
      msg += node->asString ();
      Logger::information (msg, FILE_LOCATION);
   );
   
   FlatEntry* flatEntry (NULL);
   
   const naming::File* file (node);
   
   if ((flatEntry = find (node)) == NULL) {
      flatEntry = new FlatEntry (file);
      a_entries.add (flatEntry);
   }      

   flatEntry->add (node);    
}



