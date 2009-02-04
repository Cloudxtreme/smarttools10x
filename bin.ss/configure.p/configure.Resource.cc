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

#include <nemesis.Logger.h>
#include <nemesis.functions.h>
#include <nemesis.defines.h>

#include <nemesis.app.functions.h>

#include <smart.configure.Resource.h>
#include <smart.configure.Vendor.h>
#include <smart.configure.Version.h>
#include <smart.configure.Outline.h>
#include <smart.configure.Application.h>
#include <smart.configure.Tuning.h>

using namespace std;
using namespace nemesis;
using namespace smart;

void configure::Resource::initialize (const std::string& path)
   throw (RuntimeException)
{  
   a_name = "GMAKE_";
   
   const char* id = getId ().c_str ();
   
   while (*id) 
      a_name += toupper (*id ++);   

   LOGINFORMATION (
      string msg ("smart::configure::Resource::initialize | Id: ");
      msg += getId ();
      msg += " | Name: ";
      msg += a_name;
      Logger::information (msg, FILE_LOCATION);
   );   
 
   read (path, io::Directory::Mode::ShortPath);
   
   Successor* _successor;
   string aux;
   
   for (io::Directory::const_iterator ii = begin (), maxii = end (); ii != maxii; ii ++) {
      const std::string& entry = io::Directory::data (ii);
      
      if (entry == "this.include" || entry == ".include")
         continue;
   
      if (a_mode == Mode::Normal) {
   //      if (entry == "none.include")
   //         continue;
         _successor = new Vendor (entry);
      }
      else {
         Version* version = new Version (Version::normalizeName (entry), Mode::NoVendor);
         _successor = version;
         a_options.push_back (version);
      }
         
      aux = path;
      aux += '/';                              
      add (_successor);
      _successor->initialize (aux += entry);      
   }
   
   if (a_mode == Mode::Normal) {
      const Successor* cvendor;
      const Version* cversion;
      for (const_successor_iterator ii = successor_begin (), maxii = successor_end (); ii != maxii; ii ++) {
         cvendor = successor (ii);
         for (const_successor_iterator jj = cvendor->successor_begin (), maxjj = cvendor->successor_end (); jj != maxjj; jj ++) {
            cversion = static_cast <const Version*> (successor (jj));
            if (cvendor->isNone () == false)
               a_options.push_back (cversion);   
            else 
               a_options.insert (a_options.begin (), cversion);
         }
      }   
   }
}

const configure::Version* configure::Resource::configure () const
   throw (RuntimeException)
{  
   LOGINFORMATION (
      string msg ("smart::configure::Resource::configure | Id: ");
      msg += getId ();
      Logger::information (msg, FILE_LOCATION);
   );   
   
   const Version* result (NULL);
   
   const int maxopt (a_options.size ());
   
   if (maxopt == 0) {
      string msg (getTitle ());
      msg += " (";
      msg += a_name;
      msg += ") | No tiene opciones disponibles";
      throw RuntimeException (msg, FILE_LOCATION);
   }
   
   int opt;
   
   if (maxopt == 1) {
      result = a_options [0];
      cout << "Seleciona " << getTitle () << " (" << a_name << "): " << result->getTitle () << endl << endl;
   }
   else 
      result = configureFromTuning (maxopt);
      
   while (result == NULL) {
      paint ();
      
      cin >> opt;
      
      if (opt > 0 && opt <= maxopt) {
         result = a_options [opt - 1];
         break;
      }    
      
      cout << endl;
   } 

   result->set ();
   
   cout << endl;
   
   return result;
}

const configure::Version* configure::Resource::configureFromTuning (const int maxopt) const
   throw (RuntimeException)
{  
   const Version* result (NULL);
   
   const configure::Application& app = static_cast <const configure::Application&> (app::functions::getApp ());
   const Tuning* tuning;

   if ((tuning = app.getTuning ()) == NULL)
      return NULL;
   
   Tuning::const_iterator oo = tuning->find (a_name);
   
   if (oo == tuning->end ()) {
      LOGWARNING (
         string msg ("Variable de ajuste '");
         msg += a_name;
         msg += "' no registrada";
         Logger::warning (msg, FILE_LOCATION);
      );            
      return NULL;
   }
   
   const Tuning::Selection& selection = Tuning::selection (oo);
      
   int opt = selection.getKey ();

   if (opt > 0 && opt <= maxopt) 
      result = a_options [opt - 1];
   
   const string* value = selection.getValue ();
   
   if (value != NULL) {
      for (const_option_iterator ii = option_begin (), maxii = option_end (); ii != maxii; ii ++) {
         if (option (ii)->getValue () == *value) {
            result = option (ii);
            break;
         }
      }
   }

   if (result != NULL)
      cout << "Ajuste " << getTitle () << " (" << a_name << "): " << result->getTitle () << endl;
      
   return result;
}

void configure::Resource::paint () const
   throw ()
{  
   int noption = 1;

   cout << "Seleccione " << getTitle () << " (" << a_name << "): " << endl;

   for (const_option_iterator ii = option_begin (), maxii = option_end (); ii != maxii; ii ++) {
      option (ii)->paint (noption ++);
      cout << endl;
   }
   
   cout << endl << "Seleccione una opcion: " << flush;
}
   
std::string configure::Resource::serialize () const 
   throw () 
{ 
   string result (Directory::getId ());
   
   result += functions::asText (":", (int) a_onTarget);   
   return result += functions::asText (":", (int) a_enableILS);   
}
