/*
   checksum - Version 10.0

   Desarrollado por la division de Nuevos conceptos de Plataformas.
   (c) Copyright 2005-2008 Telefonica Investigacion y Desarrollo. S.A. Unipersonal.

   Calcula la clave numérica correspondiente a la tupla (path, keycode) recibidos
   como parámetro.

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
#include <sys/types.h>
#include <fcntl.h>

#include <nemesis.CommandLine.h>
#include <nemesis.functions.h>

#include <smart.functions.h>

using namespace std;
using namespace nemesis;
using namespace smart;

//----------------------------------------------------------------------------------------
// Recive el nombre de directorio a comprobar. Usa el sistema de retorno habitual en los
// comandos de S.O. asi que devuelve 0 si hay que procesarlo o 1 en caso contrario. 
//----------------------------------------------------------------------------------------
int main (const int argc, const char* argv [])
{
   CommandLine& cl (CommandLine::instanciate ());
   int keycode = 0;

   try {
      cl.add ("f", CommandLine::Argument::Optional, "Path del fichero a comprobar");
      cl.add ("kc", CommandLine::Argument::Optional, "Key code");
      cl.add ("tc", CommandLine::Argument::Optional, "Text Code");
      
      cl.initialize (argv, argc);
      cl.verify ();

      if (cl.exists ("kc"))
         sscanf (cl.getValue ("kc"), "0x%x", &keycode);
      else if (cl.exists ("tc"))
         keycode = smart::functions::keycode (cl.getValue ("tc"));
                
      if (keycode == 0) {
         cerr << "Hay que indicar la clave o el texto con la que calcularla" << endl << endl;
         exit (1);
      }

//      cout << "keycode: " << nemesis::functions::asHexString (keycode) << " ";
      cout << nemesis::functions::asHexString (keycode) << " ";
      
      if (cl.exists ("f")) {
         keycode = smart::functions::keyfile (cl.getValue ("f"), keycode);
//         cout << "kfile: " << nemesis::functions::asHexString (keycode) << " ";
         cout << nemesis::functions::asHexString (keycode) << " ";
      }
      
      cout << endl;
   }
   catch (Exception& ex) {
      cerr << ex.asString () << endl;
   }   
     
   return 0;
}
