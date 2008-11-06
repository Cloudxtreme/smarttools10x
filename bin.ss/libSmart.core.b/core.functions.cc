/*
   Smart.core - Core de las aplicaciones Smart. 
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

#ifdef __solaris__
#include <sys/systeminfo.h>
#endif
   
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/utsname.h>
#include <fcntl.h>
#include <unistd.h>

#include <nemesis.defines.h>
#include <nemesis.Logger.h>
#include <nemesis.functions.h>
#include <nemesis.Tokenizer.h>

#include <smart.functions.h>

using namespace std;
using namespace nemesis;

string smart::functions::execute (const char* command) 
   throw (RuntimeException)
{
   string result;
   char buffer [1024];
   FILE* child_out;

   if ((child_out = popen (command, "r")) == NULL) { 
      const int xerror = errno; 
      string msg ("smart::functions::execute | Command: ");
      msg += command;
      throw RuntimeException (msg, xerror, FILE_LOCATION);
   }

   while (fgets (buffer, sizeof (buffer), child_out) != NULL)
      result += buffer;

   pclose (child_out);

   string::size_type pos = result.find_last_of ('\n');

   if (pos != string::npos)
      result.erase (pos, 1);

   LOGINFORMATION (
      string msg ("smart::functions::execute | Command: '");
      msg += command;
      msg += "' | Result: ";
      msg += result;
      Logger::information (msg, FILE_LOCATION);
   );

   return result;
}

string smart::functions::getSystemId () 
   throw ()
{
   string result (getOSName ());
   return result += getArchitecture ();
}

/*static*/
std::string smart::functions::getArchitecture () 
   throw ()
{
   string result;

#ifdef __solaris__   
   char aux [64];

   if (sysinfo (SI_ARCHITECTURE, aux, sizeof (aux)) == -1)
      nemesis_strcpy (aux, "sparc");

   /* Problema detectado en Solaris, pero no podemos cambiar porque el impacto sería grande */
   result = (nemesis_strcmp (aux, "sun4u") == 0) ? "sparc": aux;
#else
   struct utsname name;
   uname (&name);
   result = name.machine;
#endif

   return result;
}

int smart::functions::keycode (const char* str)
   throw ()
{
   int result (0xfffefdfc);
   int nbit = 0;
   int byte;   
   
   while (*str != 0) {
      byte = ((result & (0xff << nbit)) >> nbit) & 0xff;
      byte += nbit;
      
      if ((byte ^= *str) != 0) {
         result &= ~(0xff << nbit);
         byte <<= nbit;
         result |= byte;         
      }
      
      if ((nbit += 8) >= sizeof (int) * 8)
         nbit = 0;     
     
      str ++;
   }
   
   return result;
}

int smart::functions::keyfile (const char* filename, const int keycode)
   throw ()
{
   struct stat data;
   
   int fd = open (filename, O_RDONLY);
   
   if (fd == -1) 
      return 0;
   
   off_t length = lseek (fd, 0, SEEK_END);
   
   if (length <= 0)
      length = 1;
   
   if (fstat (fd, &data) != 0) {
      close (fd);
      return 0;
   }

   close (fd);
   
   unsigned int code = data.st_mtime * length; 
   
   code ^= keycode;
   
//   cout << endl << "mtime: " << data.st_mtime << " L: " << length << " code: " << code << endl;
   
   string textcode (nemesis::functions::asString (code));   
   return functions::keycode (textcode.c_str ());   
}

// 
// Devuelve el nombre del sistema operativo y los dos primeros digitos de la release.
// si el numero de relese no contiene ningun punto => elimina los caracteres que 
// puedieran causar errores de interpretacion al crear un nombre de archivo en base
// a este nombre
//
string smart::functions::getOSName ()
   throw ()
{
   struct utsname name;
   
   uname (&name);
   
   string result (name.sysname);
   
   char* release = nemesis_strchr (name.release, '.');
   
   if (release != NULL)
      if ((release = nemesis_strchr (release + 1, '.')) != NULL)
         *release = 0;
   
   return result += name.release;
}

