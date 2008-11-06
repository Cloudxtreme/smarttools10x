/*
   configure - Version 10.
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

#include <nemesis.CommandLine.h>

#include <smart.configure.Application.h>

using namespace std;
using namespace nemesis;
using namespace smart;

int main (int argc, const char** argv)
{
   CommandLine& commandLine (CommandLine::instanciate ());
   configure::Application application;
   int result (0);

   try {
      commandLine.initialize (argv, argc);
      commandLine.verify ();
      application.start ();
   }
   catch (Exception& ex) {
      cout << ex.asString () << endl;
      result = 1;
   }
   
   return result;
}

