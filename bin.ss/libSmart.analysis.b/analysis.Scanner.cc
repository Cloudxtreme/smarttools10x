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

#include <nemesis.Logger.h>

#include <nemesis.io.Directory.h>
#include <nemesis.io.functions.h>
#include <nemesis.io.TextReader.h>

#include <smart.Variable.h>

#include <smart.naming.File.h>
#include <smart.naming.FileClass.h>

#include <smart.description.RuleAgent.h>
#include <smart.description.Target.h>
#include <smart.description.functions.h>

#include <smart.analysis.FileSystem.h>
#include <smart.analysis.Scanner.h>
#include <smart.analysis.CSource.h>

#include <smart.analysis.filesystem.Path.h>
#include <smart.analysis.filesystem.FlatDirectory.h>
#include <smart.analysis.filesystem.FlatEntry.h>

using namespace std;
using namespace nemesis;
using namespace smart;

const description::RuleAgent* analysis::Scanner::st_ruleAgent (NULL);

analysis::Scanner::Scanner (analysis::FileSystem* fileSystem, const smart::Variable* variable) :
   Runnable (variable->getName ()),
   a_fileSystem (*fileSystem),
   a_exception (NULL)
{
   using namespace analysis::filesystem;

   Path::ScanMode::_v scanMode;
   
   scanMode = (nemesis_strcmp (variable->getName (), "MYPJ") == 0) ? Path::ScanMode::Full: Path::ScanMode::Quick;
   
   a_path = new Path (variable, scanMode);
}

void analysis::Scanner::execute () 
   throw ()
{
   try {
      a_thread.start (*this);
   }
   catch (Exception& ex) {
      ex.trace ();
   }
}

void analysis::Scanner::verify () const
   throw (RuntimeException)
{
   if (a_exception != NULL)
      throw *a_exception;
      
   using namespace smart::analysis::filesystem;

   const FlatDirectory* fdir = a_path->getFlatDirectory ();
   const FlatEntry* fentry (NULL);
   FlatDirectory::const_entry_iterator ii, maxii;

   for (ii = fdir->entry_begin (), maxii = fdir->entry_end (); ii != maxii; ii ++) {
      if (FlatDirectory::entry (ii)->path_size () > 1) {
         string msg ("Nombre de archivo duplicado: ");
         msg += FlatDirectory::entry (ii)->asString ();
         Logger::warning (msg, FILE_LOCATION);
      }
   }
}

//---------------------------------------------------------------------------------
// Metodo invocados desde nemesis::Thread, por lo que corre en su propio thread
//---------------------------------------------------------------------------------
void analysis::Scanner::initialize () 
   throw (RuntimeException)
{
   a_fileSystem.scannerInitialize ();   
   Runnable::initialize ();
}

void analysis::Scanner::run () 
   throw (RuntimeException)
{
   LOGINFORMATION (
      string msg ("smart::analysis::Scanner::run | ");
      msg += asString ();
      msg += " | ScanMode: ";
      msg += (a_path->getScanMode () == filesystem::Path::ScanMode::Full) ? "Full": "Quick";
      Logger::information (msg, FILE_LOCATION);
   );

   string aux;

   try {
      const description::Rule* rule = description::RuleAgent::instance ().find (a_path->getFileClass ());
      
      if (rule == NULL) {
         string msg (a_path->asString ());
         msg += " | No es una fuente de analisis reconocida";
         throw RuntimeException (msg, FILE_LOCATION);
      }
   
      a_path->setRule (rule);
      
      forward (a_path, aux);
   }
   catch (RuntimeException& ex) {
      a_exception = new RuntimeException (ex);
   }
}

void analysis::Scanner::terminate () 
   throw ()
{
   Runnable::terminate ();

   try {
      a_fileSystem.scannerTerminate ();
   }
   catch (Exception& ex) {
      ex.trace ();
   }
}

//-------------------------------------------------------------------------------------------
// Metodo estatico recursivo
//
// (1) Conviene evitar tener que llamar al getVariablePath antes de tiempo.
// (2) El unico componente que no tenga extension y que pudiera interesar analizar seria un
//     directorio hdrs => si no tiene extension y el paso anterior no lo ha reconocido =>
//     no hay que analizarlo.
// (3) Si no es un hdrs y si coincide con la extension de alguna regla que implica directorios =>
//     verifica que realmente la entrada corresponde a un directorio, para recorrerlo.
// (4) Si no encuentra ninguna regla que aplicar => ignora el nodo.
// (5) Si los elementos no tiene relacion => la interseccion de sus mascara sera 0.
//-------------------------------------------------------------------------------------------
/*static*/
void analysis::Scanner::forward (filesystem::Path* path, string& aux)
   throw (RuntimeException)
{
   io::Directory directory;
   io::Directory::const_iterator ii, maxii;
   const naming::FileClass* fileClass;
 
   FileSystem& fileSystem = FileSystem::instanciate ();
   
   if (fileSystem.exclude (path) == true)
      return;            
   
   LOGINFORMATION (
      string msg ("smart::analysis::Scanner::forward | ");
      msg += path->asString ();
      msg += " | ";
      msg += path->getRule ()->asString ();
      Logger::information (msg, FILE_LOCATION);
   );

   directory.read (path->getFullPath (), io::Directory::Mode::ShortPath);
   
   naming::File file (path);
   
   aux = path->getFullPath ();
   aux += "/makefile.order";
   
   io::TextReader makefile_order (aux);
   
   if (makefile_order.isok ()) {
      order_container order;
      order_iterator oo, maxoo;
      
      preload (makefile_order, order, aux);
      
      maxii = directory.end ();
      ii = directory.begin ();      
      for (oo = order.begin (), maxoo = order.end (); oo != maxoo; oo ++) {
         string& fileName (*oo);         
         if (std::find (ii, maxii, fileName) == maxii) {
            string msg (path->getName ());
            msg += "/makefile.order | Fichero: ";
            msg += fileName;
            msg += " | No existe en el directorio";
            Logger::warning (msg, FILE_LOCATION);
            cout << msg << endl;      
            continue;
         }
         file.replaceName (fileName);
         analize (path, file, aux, true);
      }
      
      oo = order.begin ();
      maxoo == order.end ();      
      for (ii = directory.begin (), maxii = directory.end (); ii != maxii; ii ++) {
         const string& fileName (io::Directory::data (ii));      
         if (std::find (oo, maxoo, fileName) == maxoo) {
            file.replaceName (fileName);
            analize (path, file, aux, false);
         }
      }      
   }
   else {   
      for (ii = directory.begin (), maxii = directory.end (); ii != maxii; ii ++) {
         file.replaceName (io::Directory::data (ii));
         analize (path, file, aux, false);
      }
   }
}  
  
//------------------------------------------------------------------------------------------------------------
// (1) Si es un repositorio de includes (hdrs) lo carga y termina la recursion 
// (2) Si no hay ningun regla para la extension recibida => ignora el archivo.
//     2.1 intenta tratar los directorios que estén dentro de hdrs, para tratar include/acm o hdrs/acm de la
//         misma forma que trata el "hdrs"
// (3) Cada regla tiene un tipo, y una mascara que indican los tipos con los que tiene relacion => si no hay
// relacion entre ambas reglas su 'and' sera 0.
// (4) Si la regla corresponde con una regla de directorio => continua profundizando en la estructura.
// (5) El directorio desde el que invocamos a este ejecutable tiene activo el modo 'Full', lo que significa
// que no solo registrara .h sino que tambien registrara los archivos fuente.
//------------------------------------------------------------------------------------------------------------
void analysis::Scanner::analize (filesystem::Path* path, naming::File& file, string& aux, const bool manualOrder)
   throw (RuntimeException)
{
   LOGDEBUG (
      string msg ("Analizando | ");
      msg += file.asString ();
      msg += functions::asText (" | ManualOrder: ", manualOrder);
      Logger::debug (msg, FILE_LOCATION);
   );

   if (st_ruleAgent->recognizeHeaderRepository (file)) {                               // (1)
      if (io::functions::isADirectory (file.getFullPath ()) == true) {
         filesystem::Path* npath = new filesystem::Path (
            path, file, st_ruleAgent->getRuleHeaderRepository (), false        
         );
         forward (npath, aux);
      }
      return;
   }

   const naming::FileClass* fileClass (file.getFileClass ());

   const description::Rule* rule (NULL);
   
   if ((rule = st_ruleAgent->find (fileClass)) == NULL) {                               // (2)
      for (filesystem::Path* ww = path; ww != NULL; ww = ww->getPath ()) {               // (2.1)
         LOGDEBUG (
            string msg ("backward | ");
            msg += ww->asString ();
            Logger::debug (msg, FILE_LOCATION);
         );
         if (st_ruleAgent->recognizeHeaderRepository (*ww) == true) {
            if (io::functions::isADirectory (file.getFullPath ()) == true) {
               filesystem::Path* npath = new filesystem::Path (
                  path, file, st_ruleAgent->getRuleHeaderRepository (), false        
               );
               forward (npath, aux);
            }
         }
      }
      return;
   }

   const int mask = path->getRule ()->getMask ();

   if ((mask & rule->getType ()) == 0) {                                               // (3)
      LOGINFORMATION (
         string msg (functions::asHexText ("No-RelationShip | Mask: ", mask));
         msg += " | ";
         msg += file.getVariablePath ();
         msg += " | ";
         msg += rule->asString ();
         Logger::information (msg, FILE_LOCATION);
      );         
      return;
   }

   if (rule->isAPath () == true) {                                                     // (4)
      if (io::functions::isADirectory (file.getFullPath ()) == true) {
         filesystem::Path* npath = new filesystem::Path (path, file, rule, manualOrder);
         forward (npath, aux);
      }
      return;
   }   

   if (rule->getType () == description::Rule::Type::Header) 
      path->create (file, rule, false);
   else if (rule->generateHeader () == true) 
      expand (rule, path->create (file, rule), file);
   else if (path->getScanMode () == filesystem::Path::ScanMode::Full)                  // (5)
      path->create (file, rule, manualOrder);
}

//----------------------------------------------------------------------------------------------------
// Expande un archivo fuente en los diferentes archivos que se generanria en el momento de 
// realizar el primero paso de la compilacion de este archivo. Por ejemplo un IDL puede generar varios 
// .h's y varios archivos C.
//
// (1) Verifica que el directorio _$(TARGET.O) ya existe en el path a tratar, ya que todos los 
// objetos intermedios iran a este directorio.
// (2) Creamos la depdencia entre este nodo intermedio que acabamos de crear y nodo original.
// (3) Lo marca como "ya interpretado" para evitar el error que se produciria cuando vaya 
// a intentar leer el archivo si todavia no existe. El archivo no existira hasta despues de haber
// ejecutar el 'make' la primera vez.
//----------------------------------------------------------------------------------------------------
void analysis::Scanner::expand (const description::Rule* rule, filesystem::Node* node, naming::File& file)
   throw (RuntimeException)
{
   const string& ptargetName (description::functions::getTargetDirectory ());
   filesystem::Path* ptarget;
   analysis::CSource* csource;
   
   LOGDEBUG (
      string msg ("smart::analysis::Scanner::expand | ");
      msg += rule->asString ();
      msg += " | Node: ";
      msg += node->getVariablePath ();
      Logger::debug (msg, FILE_LOCATION);
   );
      
   ptarget = static_cast <filesystem::Path*> (node->getPath ()->successor_find (ptargetName));

   if (ptarget == NULL) {                                                                    // (1)
      naming::File target_o (file);
      target_o.replaceName (ptargetName);
      ptarget = new filesystem::Path (node->getPath (), target_o, NULL, false);
      ptarget->setScanMode (filesystem::Path::ScanMode::Virtual);
   }
   
   const description::Target* target;
   
   const naming::File backup (file);
   
   for (description::Rule::const_target_iterator ii = rule->target_begin (), maxii = rule->target_end (); ii != maxii; ii ++) {
      target = description::Rule::target (ii);
      target->rename (file);      
      
      csource = static_cast <CSource*> (ptarget->create (file, target->getRule ()));
      csource->firststep_add (node);                                                            // (2)
      csource->interpreted ();                                                                  // (3)
      
      LOGDEBUG (
         string msg ("analysis::Scanner::expand | New: ");
         msg += csource->asString ();
         Logger::debug (msg, FILE_LOCATION);
      );
      
      file = backup;                                           
   }
}

//---------------------------------------------------------------------------------------------------
// Carga el archivo makefiles.order y lo carga en un vector que indicara el orden de analisis
// de los componentes del directorio que esta procesando.
//---------------------------------------------------------------------------------------------------
void analysis::Scanner::preload (io::TextReader& makefile_order, order_container& order, string& aux) 
   throw (RuntimeException)
{   
   char* line;
   char* w;
   
   while ((line = const_cast <char*> (makefile_order.fetch ())) != NULL) {
      if ((w = nemesis_strchr (line, '#')) != NULL)
         *w = 0;

      if ((w = nemesis_strchr (line, '\n')) != NULL)
         *w = 0;

      if (nemesis_strlen (line) == 0)
         continue;

      aux = line;
      LOGDEBUG (
         string msg ("analysis::Scanner::preload | Item: ");
         msg += aux;
         Logger::debug (msg, FILE_LOCATION);
      );
      order.push_back (aux);
   }
}

