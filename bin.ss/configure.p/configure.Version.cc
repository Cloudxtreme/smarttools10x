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
#include <stdlib.h>

#include <nemesis.Logger.h>

#include <nemesis.app.functions.h>

#include <smart.configure.Application.h>
#include <smart.configure.Resource.h>
#include <smart.configure.Vendor.h>
#include <smart.configure.Version.h>

using namespace std;
using namespace nemesis;
using namespace smart;

void configure::Version::initialize (const std::string&)
   throw (RuntimeException)
{  
   if (getPredecessor ()->isNone () == false) {
      a_title = getPredecessor ()->getId ();
      
      if (a_mode == Mode::Normal) {         
         a_value = a_title;
         a_value += '/';
         a_value += getId (); 
      }
      else
         a_value = getId ();
      
      a_title += ' ';
      a_title += getId (); 
   }
   else {
      a_title = getId ();
      a_value = "none";
   }

   LOGINFORMATION (
      string msg ("smart::configure::Version::initialize | Id: ");
      msg += getId ();
      msg += " | Value: ";
      msg += a_value;
      Logger::information (msg, FILE_LOCATION);
   );   
}

void configure::Version::paint (const int noption) const
   throw ()
{
   cout << noption << ".- " << a_title;
}

const configure::Resource* configure::Version::getResource () const 
   throw ()
{
   return ((a_mode == Mode::Normal) ? getVendor ()->getResource (): (Resource*) getPredecessor ());
}

void configure::Version::set () const
   throw ()
{
   const Resource* resource (getResource ());

   const std::string& varname = resource->getName ();
      
   LOGINFORMATION (
      string msg ("smart::configure::Version::set | Variable: ");
      msg += varname;
      msg += " | Value: ";
      msg += a_value;
      Logger::information (msg, FILE_LOCATION);
   );   
   
   static_cast <configure::Application&> (app::functions::getApp ()).write (resource, varname, a_value);
}

std::string configure::Version::normalizeName (const std::string& entry) 
   throw ()
{
   string result;
   string::size_type pos;
   const char* str;
   
   if ((pos = entry.find_last_of ('.')) != string::npos) 
      result = entry.substr (0, pos);  
   else
      result = entry;
   
   return result;
}



