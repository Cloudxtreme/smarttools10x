/*
   generate - Version 10.
   Herramienta que crea los archivos de dependencias necesarios para crear de forma optima los ejecutables.

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

#include <nemesis.io.functions.h>

#include <smart.Variable.h>

#include <smart.naming.File.h>
#include <smart.naming.FileClass.h>

#include <smart.description.RuleAgent.h>
#include <smart.description.functions.h>

#include <smart.structure.sccs.h>

#include <smart.analysis.FileSystem.h>
#include <smart.analysis.CSource.h>

#include <smart.analysis.filesystem.FlatEntry.h>
#include <smart.analysis.filesystem.Path.h>

#include <smart.generate.sccs.h>
#include <smart.generate.defines.h>
#include <smart.generate.Application.h>
#include <smart.generate.AutoPointer.h>
#include <smart.generate.Agent.h>
#include <smart.generate.Prototype.h>

using namespace std;
using namespace nemesis;
using namespace smart;

smart_import_sccs (Generate)

generate::Application::Application () : 
   analysis::Application ("SmartTools.Generate", "Generador de dependencias", smart_use_sccs_version (Generate))
{
   structure::sccs::activate ();
   generate::sccs::activate ();
}

void generate::Application::run () 
   throw (RuntimeException)
{
   analysis::Application::run ();
      
   cout << endl;
   defines::setUseProjectConfig (testFineCongurator (description::functions::getProjectConfig ()));
   defines::setUseTuneProjectConfig (testFineCongurator (description::functions::getTuneProjectConfig ()));
   cout << endl;
   
   {
      AutoPointer prototype;   
      prototype = Agent::instance (NULL, getcwd ());
      prototype->execute ();
   }
      
   analysis::Application::showTime (functions::milisecond ());
}

/* static */
bool generate::Application::testFineCongurator (const std::string& filename)
   throw (RuntimeException)
{
   analysis::FileSystem& fileSystem (analysis::FileSystem::instance ());
   
   string globalConfig (fileSystem.findVariable ("BASEPJ")->getValue ());
   globalConfig += "/";
   globalConfig += filename;
   
   cout << "Usando " << globalConfig << " ... " << flush;
   
   const bool result = io::functions::exists (globalConfig);
   
   cout <<  (result ? "Ok": "No") << endl;
   
   return result;
}

