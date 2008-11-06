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

#include <smart.analysis.filesystem.FlatEntry.h>
#include <smart.analysis.filesystem.Node.h>

using namespace std;
using namespace nemesis;
using namespace smart;
using namespace smart::analysis;

void filesystem::FlatEntry::add (filesystem::Node* node) 
   throw (RuntimeException)
{
   for (path_iterator ii = path_begin (), maxii = path_end (); ii != maxii; ii ++) {
      if (path (ii) == node) 
         return;
   }
        
   LOGINFORMATION (
      string msg ("smart::analysis::filesystem::FlatEntry::add | ");
      msg += node->asString ();
      Logger::information (msg, FILE_LOCATION);
   );
      
   if (a_maxPath == MaxPath) {
      string msg ("analysis::filesystem::FlatEntry::add  | ");
      msg += node->asString ();
      msg += functions::asText (" | Sobrepasada el numero maximo de ocurrencias (", MaxPath);
      msg += ')';
      throw RuntimeException (msg, FILE_LOCATION);
   }
   
   a_fullPaths [a_maxPath ++] = node;
}

const filesystem::Node* filesystem::FlatEntry::getPath (const int pos) const 
   throw (RuntimeException)
{
   const_path_iterator ii = path_begin () + pos;
   
   if (ii >= path_end ()) {
      string msg ("smart::analysis::filesystem::FlatEntry::getPath | ");
      msg += asString ();
      msg += functions::asText (" | Pos: ", pos);
      msg += functions::asText (" | Fuera de rango [0,", a_maxPath);
      msg += ")";
      throw RuntimeException (msg, FILE_LOCATION);      
   }
   
   return path (ii);
}

bool filesystem::FlatEntry::contains (const Node* node) const 
   throw ()
{
   for (const_path_iterator ii = path_begin (), maxii = path_end (); ii != maxii; ii ++) {
      if (path (ii) == node) 
         return true;
   }
   
   return false;
}

string filesystem::FlatEntry::asString () const 
   throw ()
{   
   string result ("smart::analysis::filesystem::FlatEntry { Name: ");
   result += getName ();
   result += " | RPath: ";
   
   if (path_size () > 1) {
      for (const_path_iterator ii = path_begin (), maxii = path_end (); ii != maxii; ii ++) {
         result += "\n\t";
         result += path (ii)->getVariablePath ();
      }
      result += "\n";
   }
   else {
      result += path (path_begin ())->getVariablePath ();
      result += ' ';
   }

   return result += "}";
}

