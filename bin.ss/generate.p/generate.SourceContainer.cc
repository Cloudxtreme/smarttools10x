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
// Un SourceContainer corresponde con la parte comun de los habituales 
// .p .b  .db .d
//--------------------------------------------------------------------
#include <vector>
#include <iostream>

#include <ctype.h>

#include <nemesis.Logger.h>
#include <nemesis.Tokenizer.h>

#include <nemesis.io.functions.h>

#include <smart.naming.FileClass.h>
#include <smart.Variable.h>

#include <smart.structure.Agent.h>
#include <smart.structure.Section.h>

#include <smart.description.functions.h>
#include <smart.description.Target.h>
#include <smart.description.Rule.h>

#include <smart.analysis.filesystem.Path.h>
#include <smart.analysis.CSource.h>

#include <smart.generate.SourceContainer.h>
#include <smart.generate.Makefile.h>
#include <smart.generate.defines.h>
#include <smart.generate.AutoPointer.h>
#include <smart.generate.Agent.h>

using namespace std;
using namespace nemesis;
using namespace smart;

string generate::SourceContainer::st_includes;

void generate::SourceContainer::initialize (Prototype* predecessor, const analysis::filesystem::Node* node) 
   throw (RuntimeException)
{
   Structure::initialize (predecessor, node);

   if (a_canContainsMakefileConfig == true) {
      string makefile_config (node->getFullPath ());
      makefile_config += '/';
      makefile_config += description::functions::getMakeConfig ();
      a_containsMakefileConfig = io::functions::exists (makefile_config);
   }
}

//---------------------------------------------------------------------------------------------------
// La variable que estamos procesando debe tener una estructura similar a esto:
//  GMAKE_STRUCTURE="config {cppcompiler {release threading rwtools library} xml ssl asn1 rdbms ss7}"
//
// (1) Crea la variable del makefile con la lista de objetos que componen el objecto (exec lib) final
//---------------------------------------------------------------------------------------------------
void generate::SourceContainer::beginning (Makefile& makefile) 
   throw (RuntimeException)
{
   Structure::beginning (makefile);

   LOGINFORMATION (
      string msg ("generate::SourceContainer::beginning | ");
      msg += a_node->asString ();
      Logger::information (msg, FILE_LOCATION);
   );
   
   writeIncludes (makefile);

   forward (static_cast <const analysis::filesystem::Path*> (a_node), a_objects);

   writeObjectList (makefile);
   do_writeObjectName (makefile);
  
   writeAllDependences (makefile);   
   writeClobber (makefile);
   do_writeDirectoryOutput (makefile);
   do_writeLoad (makefile);
   do_writeILS (makefile);
   do_writeObjectRule (makefile);
}

//--------------------------------------------------------------------------------------------------
// (1) Solo los directorios correspondientes a ejecutables y librerias pueden contener un archivo
// makefile.config => el resto de directorios dependendientes deberan cargar este mismo.
// (2) ww podria ser NULL si se genera el makefile desde un directorio .db o .d directamente
//--------------------------------------------------------------------------------------------------
void generate::SourceContainer::writeIncludes (Makefile& makefile) const 
   throw (RuntimeException)
{
   makefile.debug (FILE_LOCATION);
   makefile.writenl ("OTHER_USER_RULES=$(GMAKE_HOME)/data/empty.include\n");
   
   if (st_includes.empty ()) {
      structure::Agent& structureAgent = structure::Agent::instance ();
      const structure::Section* root = structureAgent.uploadEnvironment ();
      forward (root, root);
      st_includes += '\n';
      cout << endl;
   }
   
   makefile.debug (FILE_LOCATION);
   makefile.write (st_includes);
   
   if (defines::getUseTuneProjectConfig () == true) {
      makefile.debug (FILE_LOCATION);
      makefile.write ("include $(BASEPJ)/");
      makefile.write (description::functions::getTuneProjectConfig ());
      makefile.write ("\n\n");
   }
   
   const SourceContainer* ww = static_cast <const SourceContainer*> (this);            // (1)
   int maxn = 0;

   while (ww != NULL && ww->a_canContainsMakefileConfig == false) {                    // (2)
      ww = static_cast <const SourceContainer*> (ww->a_predecessor);
      maxn ++;
   }

   if (ww != NULL && ww->a_containsMakefileConfig == true) {
      makefile.debug (FILE_LOCATION);
      makefile.write ("include ");
      for (int n = 0; n < maxn; n ++)
         makefile.write ("../");
      makefile.write (description::functions::getMakeConfig ());
      makefile.write ("\n\n");
   }
}

void generate::SourceContainer::forward (const structure::Section* root, const structure::Section* section)
   throw (RuntimeException)
{
   string fix;

   fix = "$(";
   fix += root->getVariable ()->getName ();
   fix += ")/";      
   
   if (root != section) {
      fix += section->getName ();
      fix += "/$(";
      fix += section->getVariable ()->getName ();
      fix += ")/";
   }

   st_includes += "include ";
   st_includes += fix;
   st_includes += "this.include\n";

   structure::Section::const_resource_iterator ii, maxii;
   const structure::Resource* resource;
   
   for (ii = section->resource_begin (), maxii = section->resource_end (); ii != maxii; ii ++) {
      resource = structure::Section::resource (ii);

      if (resource->isASection () == false) {      
         st_includes += "include ";
         st_includes += fix;
         st_includes += resource->getName ();
         st_includes += "/$(";
         st_includes += resource->getVariable ()->getName ();
         st_includes += ").include\n";   
      }
      else
         forward (root, static_cast <const structure::Section*> (resource));
   }
}
  
void generate::SourceContainer::writeObjectList (Makefile& makefile) const 
   throw (RuntimeException)
{
   const std::string& targetDirectory (description::functions::getTargetDirectory ());
   const analysis::filesystem::Node* node;   
   
   makefile.debug (FILE_LOCATION);  
   makefile.write ("OBJECT_LIST=$(OTHER_USER_OBJECTS)");
   string objectList;
   
   for (const_dependence_iterator ii = a_objects.begin (), maxii = a_objects.end (); ii != maxii; ii ++) {   // (1)
      makefile.write (" \\\n\t");                 
      
      node = dependence (ii);
      
      if (node->getParent () != a_node) {
         makefile.write (node->getParent ()->getRelativePath (a_node));
         makefile.write ('/');
      }
  
      objectList = node->getRule ()->getAllOutputNames (targetDirectory, node);

      if (objectList.empty ()) {
         objectList = targetDirectory;
         objectList += '/';
         objectList += node->getRule ()->getOutputName (node);         
      }
      
      makefile.write (objectList);
   }
   makefile.newline ();
   makefile.newline ();
}

//---------------------------------------------------------------------------------------------------
// Escribe la regla inicial del proyecto. all: 
// Recordar que el arbol de dependencias se ha aplanado mediante el metodo 'forward'
//
// (1) Antes de nada debe crear todos los directorios "TARGET" que pudieran ser necesarios.
//---------------------------------------------------------------------------------------------------
void generate::SourceContainer::writeAllDependences (generate::Makefile& makefile) 
   throw (RuntimeException)
{
   const analysis::filesystem::Node* node;
   const naming::File* parent (NULL);

   const std::string& targetDirectory (description::functions::getTargetDirectory ());

   makefile.debug (FILE_LOCATION);
   makefile.write ("all : ../");
   makefile.write (targetDirectory);
   makefile.space ();
   
   if (getType () == Type::Library)
      makefile.write (" ILS ");
      
   makefile.write (targetDirectory);
   makefile.write (" $(OTHER_USER_DEPENDENCES) ");

   for (dependence_iterator ii = a_objects.begin (), maxii = a_objects.end (); ii != maxii; ii ++) {     // (1)     
      node = dependence (ii);
      
      if (parent == node->getParent ()) 
         continue;
         
      if ((parent = node->getParent ()) != a_node) {
         makefile.write (" \\\n\t");
         makefile.write (parent->getRelativePath (a_node));
         makefile.write ('/');
         makefile.write (targetDirectory);      
      }
   }
      
   makefile.write (" \\\n\t$(OBJECT_NAME)\n\n");
}

void generate::SourceContainer::do_writeLoad (generate::Makefile& makefile) const
   throw (RuntimeException)
{
   makefile.debug (FILE_LOCATION);
   makefile.write ("load : \n");
   makefile.write ("\t$(VIEWER)rm -f $(OBJECT_NAME)\n");
   makefile.write ("\t$(VIEWER)make -rsf ");
   makefile.write (description::functions::getMakeName ());
   makefile.write (" $(OBJECT_NAME)\n\n");
}

//----------------------------------------------------------------------------------------------
// Escribe las reglas de creacion de cada uno de los objetos contenidos en el directorio
//----------------------------------------------------------------------------------------------
void generate::SourceContainer::body (Makefile& makefile) 
   throw (RuntimeException)
{
   LOGINFORMATION (
      string msg ("generate::SourceContainer::body | ");
      msg += a_node->asString ();
      Logger::information (msg, FILE_LOCATION);
   );

   generate::AutoPointer prototype;

   for (dependence_iterator ii = a_objects.begin (), maxii = a_objects.end (); ii != maxii; ii ++) {
      prototype = Agent::instance (this, dependence (ii));
      prototype->execute ();
   }
}

void generate::SourceContainer::ending (Makefile& makefile) 
   throw (RuntimeException)
{
   Structure::ending (makefile);

   makefile.debug (FILE_LOCATION);   
   makefile.writenl ("include $(OTHER_USER_RULES)\n");
}

void generate::SourceContainer::clear ()
   throw ()
{
   Structure::clear ();   
   a_objects.clear ();
}

void generate::SourceContainer::forward (const analysis::filesystem::Path* path, generate::Prototype::dependence_container& objects)
   throw ()
{
   using namespace smart::analysis::filesystem;
   using namespace smart::description;     

   const Rule* rule;

   if (path->hasManualOrder () == true) {
      Path* _path = const_cast <Path*> (path);
      Path::manualorder_iterator ii, maxii;
      Node* node;
      
      for (ii = _path->manualorder_begin (), maxii = _path->manualorder_end (); ii != maxii; ii ++) {
         node =  Path::manualorder (ii);

         if ((rule = node->getRule ()) == NULL)
            continue;         

         if (rule->getType () == Rule::Type::LogicalGroup)
            forward (static_cast <const analysis::filesystem::Path*> (node), objects);
         else if (rule->isASource ()){
            LOGDEBUG (
               string msg ("Registrando dependencia | ");
               msg += path->getVariablePath ();
               msg += " | ";
               msg += node->asString ();
               Logger::debug (msg, FILE_LOCATION);
            );         
            objects.push_back (node);
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
         
      if ((rule = node->getRule ()) == NULL)
         continue;         
         
      if (rule->getType () == Rule::Type::LogicalGroup)
         forward (static_cast <const analysis::filesystem::Path*> (node), objects);
      else if (rule->isASource ()){
         LOGDEBUG (
            string msg ("Registrando dependencia | ");
            msg += path->getVariablePath ();
            msg += " | ";
            msg += node->asString ();
            Logger::debug (msg, FILE_LOCATION);
         );         
         objects.push_back (node);
      }
   }
}   

void generate::SourceContainer::do_writeDirectoryOutput (Makefile& makefile) 
   throw (RuntimeException)
{
   makefile.debug (FILE_LOCATION);
   makefile.write ('%');
   makefile.write (description::functions::getTargetExtension ());
   makefile.writenl (": ");
   
   makefile.writenl ("\t@if [ ! -d $@ ] ; then mkdir $@; fi ;\n"); 
}

