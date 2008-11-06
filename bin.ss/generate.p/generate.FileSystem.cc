/*
   generate - Version 10.
   Herramienta que crea los archivos de dependencias necesarios para crear de forma optima los ejecutables.

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
//--------------------------------------------------------------------
// Un FileSystem corresponde con los habituales .ss .pj
//--------------------------------------------------------------------
#include <nemesis.Logger.h>

#include <smart.naming.FileClass.h>

#include <smart.analysis.filesystem.Path.h>

#include <smart.description.RuleAgent.h>
#include <smart.description.Rule.h>
#include <smart.description.functions.h>

#include <smart.generate.FileSystem.h>
#include <smart.generate.Makefile.h>

using namespace std;
using namespace nemesis;
using namespace smart;

void generate::FileSystem::initialize (generate::Prototype* predecessor, const analysis::filesystem::Node* node) 
   throw (RuntimeException)
{
   Structure::initialize (predecessor, node);
   
   a_libraries.clear ();
   a_execs.clear ();
}

//-----------------------------------------------------------------------------------
// Establece la "all" con los nombres de librerias y/o ejecutables que hay
// en esta rama de directorios.
//
// (1) Si no ha recibido la informacion por el metodo copy => tiene que obtenerla
// el mismo recorriendo su estructura.
//-----------------------------------------------------------------------------------
void generate::FileSystem::body (Makefile& makefile) 
   throw (RuntimeException)
{
   LOGINFORMATION (
      string msg ("generate::FileSystem::body | ");
      msg += a_node->asString ();
      Logger::information (msg, FILE_LOCATION);
   );
   
   forward (static_cast <const analysis::filesystem::Path*> (a_node), a_libraries, a_execs);
   
   makefile.debug (FILE_LOCATION);
   makefile.write ("all :");
   writeAllDependences (makefile, a_libraries);
   writeAllDependences (makefile, a_execs);         
   makefile.newline ();
   makefile.newline ();
   
   writeClobber (makefile);

   if (a_libraryExtensions.empty ()) 
      prepareExtensions ();
   
   makefile.debug (FILE_LOCATION);
   makefile.write (a_execExtensions);
   makefile.write (a_libraryExtensions);
   makefile.writenl (": FORCE");
   makefile.writenl ("\t$(VIEWER)if [ $(EXTERNAL_TEST) -eq 0 ] ; \\");
   makefile.writenl ("\tthen \\");
   makefile.write ("\t\tif $(GMAKE_HOME)/bin/$(GMAKE_SYSTEM)/has_resources -d $@ -r ");
   makefile.write (description::functions::getMakeResource ());
   makefile.writenl (" ; \\");
   makefile.writenl ("\t\tthen \\");
   makefile.write ("\t\t\techo Generando $@; cd $@ > /dev/null; make -rsf ./");
   makefile.write (description::functions::getMakeName ());
   makefile.writenl (" ; \\");
   makefile.writenl ("\t\tfi ; \\");
   makefile.writenl ("\tfi ;\n");
}

//--------------------------------------------------------------------------------------------
// Se invoca desde generate::Structure, para tratar de reutilizar los calculos hechos en 
// etapas previas de la generacion. a -> b (b depende de a) => a = this, b = other
//--------------------------------------------------------------------------------------------
void generate::FileSystem::copy (const generate::Prototype* other)
   throw ()
{
   bool todo = true;

   if (other->getType () != Type::FileSystem) 
      todo = false;
 
    LOGINFORMATION (
      string msg ("generate::FileSystem::copy | Target: ");
      msg += a_node->asString ();
      msg += " | Source: ";
      msg += other->getNode ()->asString ();
      msg += " | Continue: ";
      msg += functions::asString (todo);
      Logger::information (msg, FILE_LOCATION);
   );

   if (todo == false)
      return;
     
   const FileSystem* fsw = static_cast <const FileSystem*> (other);

   const_dependence_iterator ii, maxii;

   for (ii = fsw->a_libraries.begin (), maxii = fsw->a_libraries.end (); ii != maxii; ii ++)
      a_libraries.push_back (dependence (ii));            

   for (ii = fsw->a_execs.begin (), maxii = fsw->a_execs.end (); ii != maxii; ii ++)
      a_execs.push_back (dependence (ii));            
}

void generate::FileSystem::clear ()
   throw ()
{
   Structure::clear ();   
   a_libraries.clear ();
   a_execs.clear ();
}

void generate::FileSystem::prepareExtensions ()
   throw ()
{
   using namespace smart::description;

   const RuleAgent& ruleAgent (RuleAgent::instance ());
   const Rule* rule;
   string* extension;
   
   RuleAgent::const_iterator ii, maxii;
   Rule::const_fileclass_iterator jj, maxjj;
   
   for (ii = ruleAgent.begin (), maxii = ruleAgent.end (); ii != maxii; ii ++) {
      rule = RuleAgent::rule (ii);
      
      extension = NULL;
      
      switch (rule->getType ()) {
         case Rule::Type::Library: extension = &a_libraryExtensions; break;
         case Rule::Type::Executable: extension = &a_execExtensions; break;
      }

      if (extension != NULL) {   
         for (jj = rule->fileclass_begin (), maxjj = rule->fileclass_end (); jj != maxjj; jj ++) {
            *extension += "%.";
            *extension += Rule::fileclass (jj)->getId ();
            *extension += ' ';            
         }
      }
   }
}

void generate::FileSystem::forward (const analysis::filesystem::Path* path, generate::Prototype::dependence_container& libs, generate::Prototype::dependence_container& execs)
   throw ()
{
   using namespace smart::analysis::filesystem;
   using namespace smart::description;     

   LOGINFORMATION (
      string msg ("generate::FileSystem::forward | ");
      msg += path->asString ();
      Logger::information (msg, FILE_LOCATION);
   );

   RuleAgent& ruleAgent (RuleAgent::instance ());
   const Rule* rule;
   
   if (path->hasManualOrder () == true) {
      Path* _path = const_cast <Path*> (path);
      Path::manualorder_iterator ii, maxii;
      Node* node;
      
      for (ii = _path->manualorder_begin (), maxii = _path->manualorder_end (); ii != maxii; ii ++) {
         node = Path::manualorder (ii);
         
         if ((rule = ruleAgent.find (node->getFileClass ())) == NULL)
            continue;

         switch (rule->getType ()) {
            case Rule::Type::FileSystem: 
               forward (static_cast <const analysis::filesystem::Path*> (node), libs, execs);
               break;
            case Rule::Type::Executable: 
               execs.push_back (node);
               break;
            case Rule::Type::Library: 
               libs.push_back (node);
               break;
         }
         
         node->written ();
      }
   }
   
   Path::const_successor_iterator ii, maxii;
   const Node* node;
   
   for (ii = path->successor_begin (), maxii = path->successor_end (); ii != maxii; ii ++) {
      node =  Path::successor (ii);
      
      if (node->isWritten () == true)
         continue;
      
      if ((rule = ruleAgent.find (node->getFileClass ())) == NULL)
         continue;
         
      switch (rule->getType ()) {
         case Rule::Type::FileSystem: 
            forward (static_cast <const analysis::filesystem::Path*> (node), libs, execs);
            break;
         case Rule::Type::Executable: 
            execs.push_back (node);
            break;
         case Rule::Type::Library: 
            libs.push_back (node);
            break;
      }
   }
}   
