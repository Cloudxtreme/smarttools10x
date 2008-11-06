/*
   hasresources - Version 10.0

   Desarrollado por la division de Nuevos conceptos de Plataformas.
   (c) Copyright 2005-2008 Telefonica Investigacion y Desarrollo. S.A. Unipersonal.

   Comprueba si la configuración activada por el usuario contiene los recursos
   indicados por los requerimientos del componente pasado para parámetro.

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

#include <sys/stat.h>

#include <iostream>

#include <nemesis.CommandLine.h>
#include <nemesis.Tokenizer.h>

#include <nemesis.io.TextReader.h>

using namespace std;
using namespace nemesis;

enum Result { True, False };

bool match (const char* component, const string& filename)
   throw (RuntimeException)
{
   struct stat statData;
   bool result (true);
   
   if (stat (filename.c_str (), &statData) == 0) {
      io::TextReader reader;
      const char* line;
      Tokenizer tk;
      Tokenizer tr;

      reader.open (filename);

      tk.activateStrip (true);
      tr.activateStrip (true);

      while ((line = reader.fetch ()) != NULL) {
         tk.apply (line, ":");

         if (tk.size () != 2) 
            continue;
            
         const string& varname = tk [0];
         const string& allValues = tk [1];

         const char* value = getenv (varname.c_str ());

         if (value == NULL) {
            value = "NULL";
            result = false;
         }
         else {
            tr.apply (allValues, " ");
            result = false;

            const int nv = nemesis_strlen (value);

            for (Tokenizer::const_iterator ii = tr.begin (), maxii = tr.end (); ii != maxii; ii ++) {
               const string& required = Tokenizer::data (ii);
               const int nr = required.length ();
                
               if (nr > nv)
                  continue;

               if (strncmp (value, required.c_str (), nr) == 0) {
                  result = true;
                  break;
               }
            }
         }
         
         if (result == false) {
            cout << component << " ignorado por falta de recursos ($" << varname << " [" << value;
            cout << "]: " << allValues << ')' << endl;
            result = false;
            break;
         }
      }
   }
   
   return result;
}

//----------------------------------------------------------------------------------------
// REcive el nombre de directorio a comprobar. Usa el sistema de retorno habitual en los
// comandos de S.O. asi que devuelve 0 si hay que procesarlo o 1 en caso contrario. 
//----------------------------------------------------------------------------------------
int main (const int argc, const char* argv [])
{
   CommandLine& cl (CommandLine::instanciate ());
   Result result (True);
   bool verbose = false;

   try {
      cl.add ("d", CommandLine::Argument::Mandatory, "Directorio a comprobar");
      cl.add ("r", CommandLine::Argument::Mandatory, "Nombre del archivo que contiene los recursos");
      cl.add ("medir", CommandLine::Argument::Optional, "Comprueba la existencia del directorio med_dir", false);
      cl.add ("v", CommandLine::Argument::Optional, "Activa el modo verbose", false);
      
      cl.initialize (argv, argc);
      cl.verify ();

      if (cl.exists ("v") == true)
         verbose = true;
      
      string filename;
      const char* component = cl.getValue ("d");
      
      filename = component;
      filename += '/';
      filename += cl.getValue ("r");
            
      if (match (component, filename) == false) {
         result = False;
         
         if (cl.exists ("medir") == true) {
            const char* target = getenv ("GMAKE_TARGET");
            
            if (target != NULL) {
               struct stat statData;
               filename = cl.getValue ("d");
               filename += "/med_dir/";
               filename += target;
               filename += ".suppress";
               
               result = (stat (filename.c_str (), &statData) == 0) ? False: True;
               
               if (result == False) 
                  cout << component << " ignorado por configuracion de usuario (obsoleto)" << endl;
            }
         }
      }
   }
   catch (Exception& ex) {
      cerr << ex.asString () << endl;
   }   
   
   if (verbose == true)
      cout << "has_resources: " << ((result == True) ? "True": "False") << " (" << result << ')' << endl;
  
   return result;
}

