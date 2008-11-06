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
#include <nemesis.Tokenizer.h>

#include <smart.description.Rule.h>

#include <smart.analysis.filesystem.FlatDirectory.h>
#include <smart.analysis.filesystem.Path.h>

#include <smart.analysis.CSource.h>
#include <smart.analysis.ASN1Source.h>

using namespace std;
using namespace nemesis;
using namespace smart;
using namespace smart::analysis;

filesystem::Path::Path (const Variable* variable, const ScanMode::_v scanMode) :
   filesystem::Node (variable),
   a_scanMode (scanMode),
   a_manualorders (NULL)
{
   a_flatDirectory = new FlatDirectory;
}

filesystem::Path::Path (filesystem::Path* parent, const naming::File& file, const description::Rule* rule, const bool manualOrder) :
   filesystem::Node (parent, file, rule),
   a_scanMode (parent->a_scanMode),
   a_manualorders (NULL)
{
   a_flatDirectory = parent->a_flatDirectory;
   parent->a_successors.add (this);
   
   if (manualOrder == true) {
      if (parent->a_manualorders == NULL) 
          parent->a_manualorders = new manualorder_container;
      parent->a_manualorders->push_back (this);      
   }
}

//----------------------------------------------------------------------------------------------
// El propio constructor enlaza la instancia creada con su predecesor, copia la referencia
// al FlatDirectory, etc, etc
//
// Recordar que los a_successor estan ordenados alfabeticamente, mientras que los a_manualorders
// estan ordenados por orden de aparacion => su orden vendra establecido por el archivo
// makefiles.order del directorio que estamos analizando.
//----------------------------------------------------------------------------------------------
filesystem::Node* filesystem::Path::create (const naming::File& file, const description::Rule* rule, const bool manualOrder)
   throw ()
{  
   filesystem::Node* result (NULL);

   switch (rule->getType ()) {
      case description::Rule::Type::Header: 
         result = new CSource (this, file, rule);
         a_flatDirectory->add (result); 
         break;
      case description::Rule::Type::AnsiC: 
         result = new CSource (this, file, rule);
         break;
      case description::Rule::Type::ASN1:
         result = new ASN1Source (this, file, rule);
         a_flatDirectory->add (result); 
   }
   
   if (result != NULL)
      a_successors.add (result);

   if (manualOrder == true) {
      if (a_manualorders == NULL)
         a_manualorders = new manualorder_container;      
      a_manualorders->push_back (result);
   }
   
   return result;
}

filesystem::Path* filesystem::Path::seek (const char* path)  
   throw (RuntimeException)
{
   filesystem::Path* result (NULL);

   if (nemesis_strchr (path, '/') != NULL) {
      Tokenizer tk (path, "/");
      filesystem::Path* w (this);
      
      for (Tokenizer::const_iterator ii = tk.begin (), maxii = tk.end (); ii != maxii; ii ++) {
         w = w->seek (Tokenizer::data (ii).c_str ());
         
         if (w == NULL)
            break;
      }
      
      result = w;
   }
   else 
      result = static_cast <filesystem::Path*> (successor_find (string (path)));
      
   return result;
}

void analysis::filesystem::Path::do_interpret () 
   throw (RuntimeException)
{
   if (a_scanMode == ScanMode::Virtual)
      return;

   for (successor_iterator ii = successor_begin (), maxii = successor_end (); ii != maxii; ii ++) 
      successor (ii)->interpret ();
}

void  analysis::filesystem::Path::do_synthesize () 
   throw (RuntimeException)
{
   for (successor_iterator ii = successor_begin (), maxii = successor_end (); ii != maxii; ii ++) 
      successor (ii)->synthesize ();
}

