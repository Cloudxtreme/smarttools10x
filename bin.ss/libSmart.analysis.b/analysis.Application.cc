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
#include <iostream>

#include <sys/stat.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <limits.h>

#include <nemesis.defines.h>
#include <nemesis.Logger.h>
#include <nemesis.CommandLine.h>
#include <nemesis.TraceWriter.h>
#include <nemesis.Tokenizer.h>
#include <nemesis.functions.h>

#include <nemesis.xml.DocumentFile.h>

#include <smart.sccs.h>
#include <smart.functions.h>
#include <smart.Variable.h>

#include <smart.description.RuleAgent.h>

#include <smart.analysis.sccs.h>
#include <smart.analysis.Application.h>
#include <smart.analysis.FileSystem.h>

#include <smart.analysis.filesystem.Path.h>

using namespace std;
using namespace nemesis;
using namespace smart;

analysis::Application::Application (const char *shortName, const char *title, const char *version) : 
   app::Application (shortName, title, version, __DATE__, __TIME__),
   a_cwd (NULL),
   a_initTime (0)
{
   analysis::sccs::activate ();

   CommandLine& ccll (CommandLine::instanciate ());
      
   ccll.add ("d", CommandLine::Argument::Optional, "Documento XML de configuracion");
   ccll.add ("v", CommandLine::Argument::Optional, "Lista de variables a incluir");
   ccll.add ("t", CommandLine::Argument::Optional, "Nivel de trazas");
   ccll.add ("x", CommandLine::Argument::Optional, "Lista de directorios a excluir");

   activateGeneralPublicLicense ();
}

void analysis::Application::initialize () 
   throw (RuntimeException)
{
   CommandLine& ccll (CommandLine::instanciate ());

   if (ccll.exists ("t") == true) 
      Logger::setLevel (Logger::asLevel (ccll.getValue ("t")));

   Logger::initialize (getShortName (), new TraceWriter (".file.trace", 4096000));   
   
   xml::DocumentFile configuration;
   
   if (ccll.exists ("d") == true) {
      configuration.initialize (ccll.getValue ("d"));
      a_configFile = ccll.getValue ("d");
   }
   else {
      Variable home ("GMAKE_HOME");      
      string config_xml = home.getValue ();
      config_xml += "/data/config.xml";
      a_configFile = config_xml;
      configuration.initialize (config_xml.c_str ());
      cout << endl;
   }
   
   description::RuleAgent::instance ().config (configuration);   
}

void analysis::Application::run () 
   throw (RuntimeException)
{
   FileSystem& fileSystem (analysis::FileSystem::instance ());
   CommandLine& ccll (CommandLine::instanciate ());
   char cwd [PATH_MAX];
   
   a_initTime = nemesis::functions::milisecond ();
   
   if (::getcwd (cwd, sizeof (cwd)) == NULL) 
      throw RuntimeException ("No se puede obtener el directorio actual", errno, FILE_LOCATION);

   cout << "Directorio actual: " << cwd << endl << endl;

   fileSystem.add ("MYPJ");
   fileSystem.add ("BASEPJ");

   if (ccll.exists ("v") == true) {
       Tokenizer v (ccll.getValue ("v"), ",");
       for (Tokenizer::const_iterator ii = v.begin (), maxii = v.end (); ii != maxii; ii ++) 
          fileSystem.add (Tokenizer::data (ii).c_str ());
   }

   if (ccll.exists ("x") == true) {
       Tokenizer v (ccll.getValue ("x"), ",");
       for (Tokenizer::const_iterator ii = v.begin (), maxii = v.end (); ii != maxii; ii ++) 
          fileSystem.exclude_add (Tokenizer::data (ii).c_str ());
   }
   
   fileSystem.load ();

   a_cwd = fileSystem.seek (cwd);   
   a_cwd->interpret ();   
   a_cwd->synthesize ();
}

void analysis::Application::showTime (const Milisecond end)
   throw ()
{   
   Milisecond totalms (end - a_initTime);
   char result [128];   
   
   Second totalSeconds = totalms / 1000;
   int hours, minutes, seconds;
   int days (0);

   hours = totalSeconds / 3600;
   totalSeconds -= (hours * 3600);

   if (hours > 24) {
      days = hours / 24;
      hours %= 24;
   }

   minutes = totalSeconds / 60;
   seconds = totalSeconds % 60;

   sprintf (result, " Proceso realizado en %d Dias, %d horas, %d minutos y %d segundos (%d ms)", days, hours, minutes, seconds, totalms);

   cout << result << endl << endl;
}

