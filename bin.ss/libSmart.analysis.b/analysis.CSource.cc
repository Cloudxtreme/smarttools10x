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
#include <ctype.h>

#include <nemesis.Logger.h>

#include <nemesis.io.TextReader.h>

#include <smart.analysis.CSource.h>
#include <smart.analysis.FileSystem.h>

#include <smart.analysis.filesystem.Path.h>
#include <smart.analysis.filesystem.FlatEntry.h>
#include <smart.analysis.filesystem.FlatDirectory.h>

using namespace std;
using namespace nemesis;
using namespace smart;

analysis::CSource::CSource (analysis::filesystem::Path* parent, const naming::File& file, const description::Rule* rule) :
   analysis::filesystem::Node (parent, file, rule),
   a_onComment (false),
   a_singleCommentIndicator ("//")
{ 
}

void  analysis::CSource::do_interpret ()
   throw (RuntimeException)
{
   io::TextReader reader;
   const char* line;
   naming::File file;
   Node* firststep;
   filesystem::Path* parent = getPath ();

   const std::string& fullpath = getFullPath ();

   reader.open (fullpath);

   while ((line = reader.fetch ()) != NULL) {
      if (*line == '\n')
         continue;     

      if (getFilename (const_cast <char*> (line), file) == false)
         continue;

      if ((firststep = search (parent, file)) == NULL) 
         continue;
         
      if (firststep == this)
         continue;

      if (a_firststeps.add (firststep) == true) {
         LOGDEBUG (  
            string msg (asString ());
            msg += " | FirstStep: ";
            msg += firststep->asString ();
            Logger::debug (msg, FILE_LOCATION);
         );
         firststep->interpret ();
      }
   }
}

//-------------------------------------------------------------------------------------
// (1) Si ya tiene registrada la dependencia con este nodo (y sus dependencias) no
// hay que volver a recorrerlo.
//-------------------------------------------------------------------------------------
void  analysis::CSource::do_synthesize ()
   throw (RuntimeException)
{   
   CSource* csource;

   include_iterator kk, maxkk;

   for (firststep_iterator ii = firststep_begin (), maxii = firststep_end (); ii != maxii; ii ++) {
      csource = static_cast <CSource*> (firststep (ii));
      
      if (csource == this)
         continue;
      
      if (a_includes.add (csource) == false)                                           // (1)
         continue;
         
      csource->synthesize ();
         
      for (kk = csource->include_begin (), maxkk = csource->include_end (); kk != maxkk; kk ++) {
         csource = static_cast <CSource*> (include (kk));
         if (csource != this)
            a_includes.add (csource);
      }
   }
   
   LOGDEBUG (  
      string msg (asString ());
      msg += " | Includes:";
      for (kk = include_begin (), maxkk = include_end (); kk != maxkk; kk ++) {
         msg += "\n\t";
         msg += include (kk)->getVariablePath ();
      }
      Logger::debug (msg, FILE_LOCATION);
   )
}

//----------------------------------------------------------------------------------------------
// Suponed uno.pj/a.ss/uno.db/hdrs
//                    /uno.db/aa.cc
//                    /dos.db/hdrs
//                    /tres.db/hdrs
//               /b.ss/otro.db/hdrs
//
// Estamos analizando aa.cc => parent = uno.db
//
// Busca el archivo ...
// 1.- Primero lo busca en los directorios que dependen del directorio que contiene el 
//     fichero que estamos analizado. Buscaria en uno.db/hdrs
// 2.- Segundo busca en los directorios que estan a la misma altura que el directorio padre
//     actual. Buscaria en la estructura de dos.db 
//
// (1) Al dar la posiblidad de buscar los .h en directorios con la pinta hdrs/acme/foom/goom.h
//     hay que tener en cuenta que es posible que se haga el include "#include foom/goom.h" o 
//     "#include acme/foom/goom.h".
// (2) Si la búsqueda detallada en base al nombre largo no dio resultado o si la búsqueda ha
//     sido en base a un nombre corto => recupera la entrada en el directorio plano y ya está-
// (3) Si el nombre del PATH contiene una subcadena igual que el nombre del archivo que estamos buscando,
//     la recupera. El nombre de archivo tendrá una forma "aaaa/zzzz/aa.h"
// (4) Las estructura detectada en prepago es:
//     .ss/hdrs/acme/goom.h
//     .ss/.cc -> acme/goom.h
//     Es decir, que el 'parent' que tenemos actualmente no tiene porqué contener el archivo que
//     estamos buscando, así que hay que hacer la búsqueda completa.
//----------------------------------------------------------------------------------------------
analysis::filesystem::Node* analysis::CSource::search (analysis::filesystem::Path* parent, const naming::File& file)
   throw (RuntimeException)
{
   using namespace smart::analysis::filesystem;

   Path* backup (parent);
   Node* result (NULL);
   bool longPath (false);
   Node* done (NULL);

   FileSystem& fs (FileSystem::instance ());        
      
   if (fs.wasNotFound (file) == false) {
      const char* filename = file.getName ().c_str ();
      const char* basename = strrchr (filename, '/');                // Recupera el último nombre ..../acme/foom/goom.h => goom.h
      
      if (basename != NULL) {                                  // si es un nombre con '/' => nombre largo
         naming::File aux (parent, basename + 1);
         
         filesystem::FlatEntry* fe = parent->getFlatDirectory ()->find (&aux);
         
         LOGDEBUG (
            string msg ("analysis::CSource::search | File: ");
            msg += filename;
            
            if (fe != NULL) {
               msg += " | ";
               msg += fe->asString ();
            }
            else {
               msg += " | FileName: ";
               msg += aux.getName ();
               msg += " | FlatEntry: <null>";
            }
            
            Logger::debug (msg, FILE_LOCATION);
         );        
         
         if (fe != NULL) {
            for (FlatEntry::path_iterator ii = fe->path_begin (), maxii = fe->path_end (); ii != maxii; ii ++) {
               if (FlatEntry::path (ii)->getVariablePath ().find (filename) != string::npos) {     
                  result = FlatEntry::path (ii);
                  break;
               }
            }            
         }
         else {                                                   // (4)
            loop_search_result rr = loop_search (parent, aux);         
            done = result_done (rr);
            result = result_found (rr);                        
         }
      }
      else {
         loop_search_result rr = loop_search (parent, file);         
         done = result_done (rr);
         result = result_found (rr);
      }
         
      if (result == NULL && done != NULL) {                         // (2)
         filesystem::FlatEntry* fe = fs.find (file, static_cast <filesystem::Path*> (done));
         if (fe != NULL){
            if (fe->path_size () > 1) {
               string msg ("Entrada ");
               msg += getVariablePath ();
               msg += " | Usa nombre duplicado: ";
               msg += fe->asString ();
               Logger::warning (msg, FILE_LOCATION);            
            }
            result = FlatEntry::path (fe->path_begin ());                        
         }
      }            
      
      if (result == NULL)
         fs.notFound (file);      
   }
   
   LOGDEBUG (
      string msg ("smart::analysis::CSource::search | File: ");
      msg += file.getName ();
      msg += " | From: ";
      msg += backup->getVariablePath ();
      msg += " | Result: ";
      msg += (result != NULL) ? result->asString ().c_str (): "<null>";
      Logger::debug (msg, FILE_LOCATION);
   );

   return result;
}

/*static*/
analysis::CSource::loop_search_result analysis::CSource::loop_search (analysis::filesystem::Path* parent, const naming::File& file)
   throw (RuntimeException)
{
   using namespace smart::analysis::filesystem;
   
   Path::successor_iterator ii, maxii;
   Node* result (NULL);
   Node* done (NULL);
   Node* w;      
   Path* p;
   
   LOGDEBUG (
      string msg ("analysis::CSource::loop_search | File: ");
      msg += file.getName ();
      Logger::debug (msg, FILE_LOCATION);
   );    
   
   do  {
      if ((result = parent->successor_find (file)) != NULL)
         break;                    

      for (ii = parent->successor_begin (), maxii = parent->successor_end (); ii != maxii; ii ++) {
         if ((w = Path::successor (ii)) == done)
            continue;

         if (w->isAPath () == true) {
            p = static_cast <Path*> (w);
            if ((result = p->successor_find (file)) != NULL)
               break;      
            if ((result = forward (p, file)) != NULL)
               break;
         }         
      }

      done = parent;
      parent = parent->getPath ();         
   } while (parent != NULL && result == NULL);
   
   return loop_search_result (result, done);
}

/*static*/
analysis::filesystem::Node* 
analysis::CSource::forward (analysis::filesystem::Path* parent, const naming::File& file)
   throw (RuntimeException)
{
   using namespace smart::analysis::filesystem;

   Path::successor_iterator ii, maxii;
   Node* result (NULL);   
   Node* w;
   Path* p;
   
   for (ii = parent->successor_begin (), maxii = parent->successor_end (); ii != maxii; ii ++) {
      w = Path::successor (ii);

      if (w->isAPath () == true) {
         p = static_cast <Path*> (w);      
         if ((result = p->successor_find (file)) != NULL)
            break;      
         if ((result = forward (p, file)) != NULL)
            break;
      }
   }
   
   return result;
}

/**
 * (1) Las lineas especiales dan la posibilidad de tratar ciertos comentarios de linea como
 * si fueran otra cosa. Por ahora han servido para indicar manualmente los archivo a incluir
 * en el procesamiento de los ASN1.
 */
bool analysis::CSource::getFilename (char* line, naming::File& file)  
   throw ()
{
   char* aux;
   
   if (specialLine (line) == false) {              // (1)
      if ((aux = nemesis_strstr (line, a_singleCommentIndicator)) != NULL)
         *aux = 0;
      
      if (*line == 0)
         return false;
   }   
        
   a_onComment = complexComment (a_onComment, line);
   
   if (a_onComment == true || *line == '\n')
      return false;
   
   return extractFile (line, file);
}

//-------------------------------------------------------------------------------------------
// Los procesa los comentarios al estilo ANSI C.
//
// (1) Si en la linea actual encuentra un '/*' pero no encuentra su correspondiente '*/' => 
//     queda en estado de comentario activo.
// (2) Si en la misma linea encuentra el '/*' y su correspondiente '*/' => Elimina lo que 
//     haya comprendido entre ellos y no entra en estado de comentario activo.
// (3) Si estaba en comentario activo y encuentra una linea con el '*/' => sale del estado 
//     de comentario activo y proceso el resto de linea que haya despues.
//-------------------------------------------------------------------------------------------
bool analysis::CSource::complexComment (const bool onComment, char* line)
   throw ()
{
   char* beginComment;
   const char* endComment;
   bool result (onComment);
   
   if (result == false) {
      beginComment = nemesis_strstr (line, "/*");
      
      if (beginComment != NULL) {                                       // (1)
         if ((endComment = nemesis_strstr (line, "*/")) == NULL) {
            *beginComment = 0;  
            result = true;
         }
         else                                                           // (2)
            nemesis_memmove (beginComment, endComment, nemesis_strlen (endComment + 2) + 1);
      } 
   }
   else if ((endComment = nemesis_strstr (line, "*/")) != NULL)  {      // (3)
      nemesis_memmove (line, endComment + 2, nemesis_strlen (endComment + 2) + 1);
      result = false;
   }         
   
   return result;
}

//-------------------------------------------------------------------------------------------------
// Procesa las inclusiones en formato C/C++ y formato ProC/ProC++
//
// (1) Si no es un #include C/C++ comprueba si es ProC.
//-------------------------------------------------------------------------------------------------
bool analysis::CSource::extractFile (char* line, naming::File& file)  
   throw ()
{
   static const int c_include_len = nemesis_strlen ("#include");
   static const int proc_exec_len = nemesis_strlen ("exec ");
   static const int proc_sql_len = nemesis_strlen ("sql ");
   static const int proc_include_len = nemesis_strlen ("include ");

   register char* aux = const_cast <char*> (nemesis_strstr (line, "#include"));

   bool result (false);
         
   if (aux != NULL) {                                                                  // (1)
      aux += c_include_len;

      while (*aux && *aux != '"' && *aux != '<')
         aux ++;

      if (*aux == 0) 
         return false;

      char* includeName = ++ aux;

      while (*aux && *aux != '"' && *aux != '>')
         aux ++;

      if (*aux) {
         *aux = 0;
         file.setName (includeName);
         result = true;
      }
   }
   else {
      while (*line && isspace (*line)) line ++;
      
      if (strncmp (line, "exec ", proc_exec_len) != 0)
         return false;
         
      if ((aux = nemesis_strstr (line, "sql ")) == NULL)
         return false;

      aux += proc_sql_len;
      
      if ((aux = nemesis_strstr (line, "include ")) == NULL)
         return false;

      aux += proc_include_len;

      while (*aux && isspace (*aux)) aux ++;
      
      if (*aux == 0)
         return false;
         
      if ((line = nemesis_strchr (aux, ';')) != NULL) {
         *line = 0;
         file.setName (aux);
         result = true;
      }
   }
   
   return result;
}
