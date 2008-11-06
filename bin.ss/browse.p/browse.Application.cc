/*
   browse - Version 10.
   Ejecutable que ayuda a visualizar los archivos que dependen de un determinado archivo de cabecera o ASN1.

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

#include <sys/stat.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

#include <nemesis.defines.h>
#include <nemesis.CommandLine.h>
#include <nemesis.functions.h>
#include <nemesis.Tokenizer.h>

#include <smart.naming.File.h>
#include <smart.naming.FileClass.h>

#include <smart.analysis.FileSystem.h>
#include <smart.analysis.CSource.h>

#include <smart.analysis.filesystem.FlatEntry.h>
#include <smart.analysis.filesystem.Path.h>

#include <smart.browse.sccs.h>
#include <smart.browse.Application.h>

using namespace std;
using namespace nemesis;
using namespace smart;

smart_import_sccs (Browse)

browse::Application::Application () : 
   analysis::Application ("SmartTools.Browse", "Visualizador de dependencias", smart_use_sccs_version (Browse))
{
   browse::sccs::activate ();
   
   CommandLine& ccll (CommandLine::instanciate ());

   ccll.add ("h", CommandLine::Argument::Mandatory, "Lista de archivos a estudiar");
}

void browse::Application::run () 
   throw (RuntimeException)
{
   analysis::Application::run ();

   analysis::FileSystem& fileSystem (analysis::FileSystem::instance ());
   CommandLine& ccll (CommandLine::instanciate ());
   
   Tokenizer token (ccll.getValue ("h"), ",");
   
   for (Tokenizer::const_iterator ii = token.begin (), maxii = token.end (); ii != maxii; ii ++) 
      process (Tokenizer::data (ii));
   
   analysis::Application::showTime (functions::milisecond ());
}

void browse::Application::process (const std::string& filename)
   throw ()
{
   using namespace smart::analysis;

   naming::File file (NULL, filename);
   filesystem::FlatEntry* fe = FileSystem::instanciate ().find (file, NULL);
   
   if (fe == NULL) {
      cout << "Archivo: " << filename << " no encontrado en el sistema de archivos" << endl << endl;
      return;
   }
   
   cout << fe->asString () << " dependencias:" << endl;
   
   forward (fe, analysis::Application::getcwd ());
   
   cout << endl;
}

void browse::Application::forward (const analysis::filesystem::FlatEntry* fe, const analysis::filesystem::Path* path)
   throw ()
{
   using namespace analysis::filesystem;
   
   Path::const_successor_iterator ii, maxii;
   const Node* node;
   const analysis::CSource* csource;
   FlatEntry::const_path_iterator kk, maxkk = fe->path_end ();
   int i;
   
   for (ii = path->successor_begin (), maxii = path->successor_end (); ii != maxii; ii ++) {
      node = Path::successor (ii);
      
      if (node->isAPath () == true) {
         forward (fe, static_cast <const Path*> (node));
         continue;
      }

      csource = static_cast <const analysis::CSource*> (node);
      
      for (kk = fe->path_begin (), i = 0; kk != maxkk; kk ++) {
         i ++;
         
         if (csource->include_contains (FlatEntry::path (kk)) == true) {
            cout << "\t(" << i << ") " << csource->getVariablePath () << endl;
            break;
         }
      }      
   }
}

