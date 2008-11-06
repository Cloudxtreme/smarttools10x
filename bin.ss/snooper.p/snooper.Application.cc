/*
   snooper - Version 10.
   Herramienta que establece las variables de entorno en base a la configuracion establecida por el usuario
   mediante la seleccion por los menus. Los menus son creados dinamicamente en base ala informacion contenida
   en el directorio de configuraciones.

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

#include <QApplication>

#include <nemesis.defines.h>
#include <nemesis.Logger.h>
#include <nemesis.CommandLine.h>
#include <nemesis.TraceWriter.h>
#include <nemesis.Tokenizer.h>

#include <nemesis.xml.DocumentFile.h>

#include <smart.Variable.h>

#include <smart.snooper.sccs.h>
#include <smart.snooper.Application.h>
#include <smart.snooper.MainWindow.h>

using namespace std;
using namespace nemesis;
using namespace smart;

smart_import_sccs (Snooper)

snooper::Application::Application () : 
   app::Application ("SmartTools.Snooper", "Visualizador de variables", smart_use_sccs_version (Snooper), __DATE__, __TIME__)
{
   snooper::sccs::activate ();

   CommandLine& ccll (CommandLine::instanciate ());
   ccll.add ("t", CommandLine::Argument::Optional, "Nivel de trazas");
   
   activateGeneralPublicLicense ();
}

void snooper::Application::initialize () 
   throw (RuntimeException)
{
   CommandLine& ccll (CommandLine::instanciate ());

   if (ccll.exists ("t") == true) 
      Logger::setLevel (Logger::asLevel (ccll.getValue ("t")));

   Logger::initialize ("snooper", new TraceWriter (".snooper.trace", 4096000));
}

void snooper::Application::run () 
   throw (RuntimeException)
{
   CommandLine& ccll (CommandLine::instanciate ());
   
   int argc (ccll.getArgc ());
   
   QApplication app (argc, (char**) ccll.getArgv ());   
   MainWindow mainWindow;
   
   mainWindow.initialize ();   
   app.exec();
}

