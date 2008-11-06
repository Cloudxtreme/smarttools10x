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
#include <nemesis.functions.h>
#include <nemesis.Logger.h>
#include <nemesis.Tokenizer.h>

#include <nemesis.io.TextReader.h>

#include <smart.configure.Outline.h>
#include <smart.configure.CppCompiler.h>

using namespace std;
using namespace nemesis;
using namespace smart;

void configure::Outline::initialize (const std::string& path)
   throw (RuntimeException)
{  
   LOGINFORMATION (
      string msg ("smart::configure::Outline::initialize | Path: ");
      msg += path;
      Logger::information (msg, FILE_LOCATION);
   );   
 
   load (path);
   read (path, io::Directory::Mode::ShortPath);
   
   Resource* resource;
   string aux;
   
   for (io::Directory::const_iterator ii = begin (), maxii = end (); ii != maxii; ii ++) {
      const string& dir = io::Directory::data (ii);
      
      const_data_iterator ii = data_find (dir);
      
      if (ii == data_end ()) 
         continue;
      
      if (dir == "cppcompiler")
         resource = new CppCompiler (dir);
      else
         resource = new Resource (dir, a_mode, onTarget (ii), enableILS (ii));      
         
      resource->setTitle (title (ii));
      aux = path;
      aux += '/';                              
      resource->initialize (aux += dir);
      
      configure::Directory::add (resource);
   }   
}

void configure::Outline::load (const std::string& path)
   throw (RuntimeException)
{
   string filename (path + "/this.outline");   
   io::TextReader reader;
   
   reader.open (filename);

   Tokenizer tokenizer;  
   const char* line;
   bool onTarget;
   bool enableILS;
      
   tokenizer.activateStrip (true);
   
   while ((line = reader.fetch ()) != NULL) {
      tokenizer.apply (line, "|");      
      
      if (tokenizer.size () < 3)
         continue;
      
      const string& id = tokenizer [1];
      
      if (data_contains (id) == true) {
         LOGWARNING (   
            string msg (filename);
            msg += " | Id: ";
            msg += id;
            msg += " | Identificador usado previamente";
            Logger::warning (msg, FILE_LOCATION);
         );
         continue;            
      }
      
      if ((onTarget = functions::asBool (tokenizer [2].c_str ())) == true)
         enableILS = (tokenizer.size () < 4) ? false:  functions::asBool (tokenizer [3].c_str ());         
      else
         enableILS = false;

      LOGINFORMATION (
         string msg ("smart::configure::Outline::load | Resource: ");
         msg += id;
         msg += " | Title: ";
         msg += tokenizer [0];
         msg += functions::asText (" | On TARGET: ", onTarget);
         msg += functions::asText (" | Enable ILS: ", enableILS);
         Logger::information (msg, FILE_LOCATION);
      );   
      
      add (id, tokenizer [0], onTarget, enableILS);
   }
}

void configure::Outline::run () const
   throw (RuntimeException)
{
   const Successor* resource;

   for (const_data_iterator ii = data_begin (), maxii = data_end (); ii != maxii; ii ++) {
      if ((resource = successor_find (id (ii))) != NULL)
         static_cast <const Resource*> (resource)->configure ();
   }
}

configure::Outline::const_data_iterator configure::Outline::data_find (const string& id) const
   throw ()
{
   const_data_iterator ii, maxii;

   for (ii = data_begin (), maxii = data_end (); ii != maxii; ii ++) {
      if (Outline::id (ii) == id)
         return ii;
   }

   return maxii;
}

std::string configure::Outline::serialize () const 
   throw ()
{
   const Successor* resource;
   string result;
   bool first = true;

   result = '{';
   
   for (const_data_iterator ii = data_begin (), maxii = data_end (); ii != maxii; ii ++) {
      if ((resource = successor_find (id (ii))) != NULL) {
         if (first == true) 
            first = false;
         else
            result += ' ';

         result += static_cast <const Resource*> (resource)->serialize ();
      }
   }
   
   return result += '}';
}

