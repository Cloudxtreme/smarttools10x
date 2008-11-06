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
#include <algorithm>

#include <stdlib.h>
#include <string.h>

#include <nemesis.Logger.h>
#include <nemesis.Guard.h>

#include <smart.Variable.h>

#include <smart.description.RuleAgent.h>

#include <smart.analysis.FileSystem.h>
#include <smart.analysis.Scanner.h>

#include <smart.analysis.filesystem.Path.h>
#include <smart.analysis.filesystem.FlatDirectory.h>
#include <smart.analysis.filesystem.FlatEntry.h>

using namespace std;
using namespace nemesis;
using namespace smart;

analysis::FileSystem::FileSystem () :
   a_semaphore (0),
   a_hasExclude (false)
{
   a_counterInitializes = a_counterTerminates = 0;
}

void analysis::FileSystem::add (const char* varname) 
   throw (RuntimeException)
{
   for (variable_iterator ii = variable_begin (), maxii = variable_end (); ii != maxii; ii ++) 
      if (nemesis_strcmp (variable (ii)->getName (), varname) == 0)
         return;

   Variable variable (varname);   

   const std::string& value = variable.getValue ();
   
   a_variables.push_back (new Variable (variable));
}

void analysis::FileSystem::exclude_add (const char* fullpath) 
   throw ()
{
   string exclude;
   
   a_hasExclude = true;
   
   const int len = nemesis_strlen (fullpath);
   
   if (fullpath [len - 1] == '/') {
      char* aux = strdup (fullpath);
      aux [len - 1] = 0;
      exclude = aux;
      free (aux);
   }
   else
      exclude = fullpath;
   
   a_excludes.push_back (exclude);
}

/**
 * Devuelve 'true' si el directorio recibido esta en la lista de directorios
 * a ignorar. Una vez que se ha encontrado se puede eliminar, porque no volvera a entrar
 * por este arbol de directorios.
 */
bool analysis::FileSystem::exclude (const analysis::filesystem::Path* path) 
   throw ()
{
   if (a_hasExclude == false)
      return false;
   
   const std::string& fullpath = path->getFullPath ();
   bool result (false);
   
   for (exclude_iterator ii = exclude_begin (), maxii = exclude_end (); ii != maxii; ii ++) {
      const string& excludepath = exclude (ii);

      if (excludepath == fullpath) {
         result = true;
         a_excludes.erase (ii);
         if (a_excludes.size () == 0)
            a_hasExclude = false;
         break;
      }         
   }

   if (result == true) {
      LOGWARNING (
         string msg ("smart::analysis::FileSystem::exclude | FullPath: ");
         msg += fullpath;
         Logger::warning (msg, FILE_LOCATION);
      );   
   }
   
   return result;
}

const smart::Variable* analysis::FileSystem::findVariable (const char* varname) const
   throw (RuntimeException)
{
   for (const_variable_iterator ii = variable_begin (), maxii = variable_end (); ii != maxii; ii ++) 
      if (nemesis_strcmp (variable (ii)->getName (), varname) == 0)
         return variable (ii);
         
   string msg ("Variable ");
   msg += varname;
   msg += " no registrada";   
   
   throw RuntimeException (msg, FILE_LOCATION);
} 

//-----------------------------------------------------------------------------------------
// (1) Optimiza la lista de variables a recorrer. Evita recorrer varias veces un mismo
// directorio. Elimina las variables que hacen referencia a directorios que ya estan
// dependendiendo de algun otro indicado por otra variable. 
//
// Suponer V1 = /a/b/c/d
//         V2 = /a/b/c/e
//         V3 = /a/b/c/x/k
//         V4 = /a/b/c
//
// Esta primera parte desactivaria el uso de las variables V1, V2 y V3 porque sus
// respectivos directorios seran incluidos al procesar el valor asociado a V4. Recordar
// que las variables estan ordenadas alfabeticamente por su valor asociado.
//
// Al lanzar los Scanner's primero los crea todos, y luego los lanza. Se hace asi para
// mantener estable el numero de Thread's durante la comprobacion de los
// scannerTerminate.
//-----------------------------------------------------------------------------------------
void analysis::FileSystem::load ()
   throw (RuntimeException)
{
   cout << endl;

   disableDuplicates ();
   
   for (variable_iterator ii = variable_begin (), maxii = variable_end (); ii != maxii; ii ++) {
      if (variable (ii)->isEnabled () == true) 
         a_scanners.push_back (new Scanner (this, variable (ii)));
   }   

   if (scanner_size () == 0) 
      throw RuntimeException ("No se ha establecido ningun directorio donde analizar", FILE_LOCATION);
      
   Scanner::setRuleAgent (description::RuleAgent::instance ());

   for (scanner_iterator ii = scanner_begin (), maxii = scanner_end (); ii != maxii; ii ++) 
      FileSystem::scanner (ii)->execute ();

   a_semaphore.wait ();

   for (scanner_iterator ii = scanner_begin (), maxii = scanner_end (); ii != maxii; ii ++) 
      FileSystem::scanner (ii)->verify ();
}

analysis::filesystem::Path* analysis::FileSystem::seek (const char* pathname) 
   throw (RuntimeException)
{
   analysis::filesystem::Path* result (NULL);
   analysis::filesystem::Path* w;
   Scanner* scn;
   const int pathlen = nemesis_strlen (pathname);
   const char* fpath;
   int fpathlen;
   
   for (scanner_iterator ii = scanner_begin (), maxii = scanner_end (); ii != maxii; ii ++) {
      if ((w = scanner (ii)->getPath ()) == NULL)
         continue;
         
      fpathlen = nemesis_strlen (fpath = w->getFullPath ().c_str ());

      if (fpath [fpathlen - 1] == '/')
         fpathlen --;
            
      if (fpathlen > pathlen)
         continue;
         
      if (strncmp (pathname, fpath, fpathlen) != 0)
         continue;

      const char* left = pathname + fpathlen;
      
      result = (*left == 0) ? w: w->seek (left);
   }

   if (result == NULL) {
      string msg ("Path: ");
      msg += pathname;
      msg += ". No localizado en el sistema de ficheros. Revise las variables PJ del entorno";
      throw RuntimeException (msg, FILE_LOCATION);
   }

   LOGINFORMATION (
      string msg ("smart::analysis::FileSystem::seek | Result: ");
      msg += result->asString ();
      Logger::information (msg, FILE_LOCATION);
   );
       
   return result;
}

//------------------------------------------------------------------------------------------------------
// Este metodo se invocara despues de no haber encontra el nodo buscado en el arbol de directorios
// recibido como parametro, por eso se pasa el arbol en el que YA se ha buscado, para evitar 
// perder tiempo buscando en un nodo en el que ya se sabe que no va a estar.
//------------------------------------------------------------------------------------------------------
analysis::filesystem::FlatEntry* analysis::FileSystem::find (const naming::File& file, const analysis::filesystem::Path* done) 
   throw (RuntimeException)
{
   filesystem::FlatEntry* result (NULL);
   filesystem::FlatDirectory* fd (NULL);
   filesystem::Node* w;   

   for (scanner_iterator ii = scanner_begin (), maxii = scanner_end (); ii != maxii; ii ++) {
      if ((w = scanner (ii)->getPath ()) == NULL)
         continue;
         
      if (w == done || w->isAPath () == false)
         continue;
         
      fd = static_cast <filesystem::Path*> (w)->getFlatDirectory ();
      
      if ((result = fd->find (&file)) != NULL) 
         break;
   }
   
   return result;  
}

void analysis::FileSystem::disableDuplicates () 
   throw (RuntimeException)
{
   variable_iterator ii, maxii, next_ii, jj;
   Variable* var;
   const char* value1;
   int len1;
   int len2;
      
   for (variable_iterator ii = variable_begin (), maxii = variable_end (); ii != maxii; ii ++) {
      if ((var = variable (ii))->isEnabled () == false)
         continue;
   
      const string& aux = var->getValue ();
      value1 = aux.c_str ();
      len1 = aux.length ();
      
      for (jj = variable_begin (); jj != maxii; jj ++) {
          if (ii == jj)
             continue;

         if ((var = variable (jj))->isEnabled () == false)
            continue;
            
         const string& aux = var->getValue ();

         if ((len2 = aux.length ()) < len1)
            continue;
   
         if (strncmp (value1, aux.c_str (), len1) == 0)
            var->disable ();
      }
   }
         
   LOGINFORMATION (  
      bool none (true);
      string msg ("smart:analysis::FileSystem::disableDuplicates | Disables: ");
      for (ii = variable_begin (), maxii = variable_end (); ii != maxii; ii ++) 
         if ((var = variable (ii))->isEnabled () == false) {
            msg += var->getName ();
            msg += ' ';
            none = false;
         }
      if (none == true)
         msg += "<none>";
      Logger::information (msg, FILE_LOCATION);
   );
}

//---------------------------------------------------------------------------------
// Registra el numero de Scanners que hemos lanzado.
//---------------------------------------------------------------------------------
void analysis::FileSystem::scannerInitialize () 
   throw (RuntimeException)
{
   Guard guard (a_mutex, "Mutex from smart::analysis::FileSystem::scannerInitialize");
   
   a_counterInitializes ++;
}

//---------------------------------------------------------------------------------
// Registra el numero de Scanner que han terminado su ejecucion.
//
// Si la cuenta de Scanner's lanzados es igual a la de parados y si ya se han 
// lanzado todos => lanza el signal que sacara al Thread principal del
// Semaphore::wait en el que debe estar parado.
//---------------------------------------------------------------------------------
void analysis::FileSystem::scannerTerminate () 
   throw (RuntimeException)
{
   Guard guard (a_mutex, "Mutex from smart::analysis::FileSystem::scannerTerminate");
   
   a_counterTerminates ++;
   
   if (a_counterInitializes == scanner_size () && a_counterInitializes == a_counterTerminates)
      a_semaphore.signal ();      
}

