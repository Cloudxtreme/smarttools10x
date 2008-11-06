/*
   ils_mkdir - Version 10.0

   Desarrollado por la division de Nuevos conceptos de Plataformas.
   (c) Copyright 2005-2008 Telefonica Investigacion y Desarrollo. S.A. Unipersonal.

   Optimiza la creación de directorios de salida de ejecutables.

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
#include <limits.h>
#include <libgen.h>
#include <unistd.h>

#include <nemesis.CommandLine.h>

using namespace std;
using namespace nemesis;

enum Result { Ignore, CreateDirectory, CreateLink };

//----------------------------------------------------------------------------------------------------------  
// (1) Si el directorio ya existe => no tenemos que hacer nada
// (2) Si la libreria pasada como parametro NO existe => el ILS no ha actuado asi que tenemos que crear
//    el directorio de forma normal.
// (3) Si la libreria NO es un link => El ILS no ha actuado => se crea el directorio
// (4) Obtiene el nombre final del TARGET contra el que el ILS hizo el link.
//----------------------------------------------------------------------------------------------------------  
int make (const char* directory, const char* library, const bool verbose)
   throw (RuntimeException)
{
   int result (Ignore);
   struct stat data;
   
   if (stat (directory, &data) == 0)                                                      // (1)
      return Ignore;
            
   if (lstat (library, &data) != 0) {                                                      // (2)
      if (errno != ENOENT) 
         throw RuntimeException (directory, errno, FILE_LOCATION);

      if (verbose) 
         cout << "Objecto " << library << " no encontrado" << endl;
      
      if (mkdir (directory, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH) != 0) 
         throw RuntimeException (directory, errno, FILE_LOCATION);         
      result = CreateDirectory;
   }
   else {
      if (S_ISLNK (data.st_mode) == 0) {                                                  // (3)
         if (verbose) 
            cout << "Objecto " << library << " no es un elemento ILS" << endl;            
         if (mkdir (directory, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH) != 0) 
            throw RuntimeException (directory, errno, FILE_LOCATION);         
         result = CreateDirectory;
      }
      else {
         char* realpath = new char [PATH_MAX];
         int n;
         
         if ((n = readlink (library, realpath, PATH_MAX)) == -1) {
            delete [] realpath;
            throw RuntimeException (library, errno, FILE_LOCATION);
         }
         
         realpath [n] = 0;
         
         if (verbose) 
            cout << "readlink: " << realpath << endl;
         
         char* target = basename (dirname (realpath));                                   // (4)         
         
         if (verbose) 
            cout << "target: " << target << endl;
         
         if (symlink (target, directory) != 0) {
            delete [] realpath;
            throw RuntimeException (target, errno, FILE_LOCATION);
         }
         
         delete [] realpath;
         
         result = CreateLink;
      }
   }
   
   return result;
}

/*
El truco para evitar que el make siga generando los .o's una vez que ya hemos creado el link a una libreria usando el ILS 
consiste en ir creando los TARGET.X del proyecto de forma que apunten como un enlace simbolico al directorio correspondiente
al que hemos hecho el link.

asi por ejemplo si  el ILS ha generado el enlace:

../_SunOS5.8sparc_Sunforte6_debug_st_oracle_none.X/libNemesis.core.a -> ../_SunOS5.8sparc_Sunforte6_debug_st_none_none.X/libNemesis.core.a

Todos los directorios de salida de los subsistemas de libNemesis.core.b deberan apuntar a los directorios de salida
de _SunOS5.8sparc_Sunforte6_debug_st_none_none.X

Asi por ejemplo
   core.b/mt.db/_linux2.4.21_gcc3.3.3_debug_st_oracle9.2.0_none.O -> core.b/mt.db/_linux2.4.21_gcc3.3.3_debug_mt_none_none.O

Con esto se consigue que cuando el make aplica las reglas para generar los .o  se encuentra que todos los .o son posteriores
a los .cc (por ejemplo) pero esta comprobando el .o a traves del link que acabamos de crear.

En el -o = ../_SunOS5.8sparc_Sunforte6_debug_st_oracle_none.X/libNemesis.core.a
   el -d = mt.db/_linux2.4.21_gcc3.3.3_debug_st_oracle9.2.0_none.X

*/
int main (const int argc, const char* argv [])
{
   CommandLine& cl (CommandLine::instanciate ());
   int result (Ignore);
   bool verbose = false;
   struct stat data;   

   try {
      cl.add ("d", CommandLine::Argument::Mandatory, "Directorio a crear");
      cl.add ("o", CommandLine::Argument::Mandatory, "Nombre del componente");
      cl.add ("v", CommandLine::Argument::Optional, "Activa el modo verbose", false);
      
      cl.initialize (argv, argc);
      cl.verify ();

      if (cl.exists ("v") == true)
         verbose = true;
         
      result = make (cl.getValue ("d"), cl.getValue ("o"), verbose);
      
   }
   catch (Exception& ex) {
      cerr << ex.asString () << endl;
   }   
   
   if (verbose == true) {
      cout << "ils_mkdir: ";
      switch (result) {
         case Ignore: cout << "Ignore"; break;
         case CreateDirectory: cout << "CreateDirectory"; break;
         case CreateLink: cout << "CreateLink"; break;
      }
      cout << " (" << result << ')' << endl;
  }
  
// siempre retorna cero, para indicar que ha terminado OK, por ahora no podemos actuar en el makefile en base
// al valor retornado por este ejecutable
   return 0;
}
