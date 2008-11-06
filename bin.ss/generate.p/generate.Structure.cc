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
#include <nemesis.Logger.h>

#include <nemesis.app.functions.h>

#include <smart.description.functions.h>

#include <smart.analysis.Application.h>
#include <smart.analysis.filesystem.Path.h>

#include <smart.generate.defines.h>
#include <smart.generate.Makefile.h>
#include <smart.generate.Agent.h>
#include <smart.generate.AutoPointer.h>
#include <smart.generate.Structure.h>

using namespace std;
using namespace nemesis;
using namespace smart;

//------------------------------------------------------------------------------------------
// Invocados desde Agent::instance y Agent::release
//------------------------------------------------------------------------------------------
void generate::Structure::initialize (generate::Prototype* predecessor, const analysis::filesystem::Node* node)
   throw (RuntimeException)
{
   Prototype::initialize (predecessor, node);
   a_makefile = Makefile::create (static_cast <const analysis::filesystem::Path*> (node));
}

void generate::Structure::clear ()
   throw ()
{
   Makefile::release (a_makefile);
   Prototype::clear ();   
}

//-----------------------------------------------------------------------------------
// Crea su Makefile correspondiente y una vez que lo ha hecho sigue navegando por
// la estructura de hijos, pera que ellos tambien creen sus Makefiles.
//
// En la segunda fase solo tiene que tratar los sucesores de tipo 'directorio' ya 
// que los otros los habra tratado la implementacion concreta del 'rules'.
//
// El primer nivel se lanza de generate::Application::run
//-----------------------------------------------------------------------------------
void generate::Structure::execute ()
   throw (RuntimeException)
{
   const analysis::filesystem::Path* path;

   if (a_node->isAPath () == false) {
      string msg ("smart::generate::Structure::execute | No se puede aplicar sobre ");
      msg += a_node->getVariablePath ();
      throw RuntimeException (msg, FILE_LOCATION);           
   }

   path = static_cast <const analysis::filesystem::Path*> (a_node);
   
   LOGINFORMATION (
      string msg ("smart::generate::Structure::execute | Path: ");
      msg += path->getFullPath ();
      Logger::information (msg, FILE_LOCATION);
   );      

   using namespace analysis::filesystem;
      
   forward (path);
      
   a_makefile->activateSection (Makefile::Section::Beginning);
   beginning (*a_makefile);

   a_makefile->activateSection (Makefile::Section::Body);
   body (*a_makefile);
   
   a_makefile->activateSection (Makefile::Section::Ending);   
   ending (*a_makefile);
}

void generate::Structure::beginning (generate::Makefile& makefile) 
   throw (RuntimeException)
{
   const analysis::Application& app = static_cast <analysis::Application&> (app::functions::getApp ());

   makefile.write ("# \n");
   makefile.write ("# Archivo generado automaticamente con ");
   makefile.write (app.getShortName ());
   makefile.write (" Version ");
   makefile.write (app.getVersion ());
   makefile.newline ();
   makefile.write ("# Componente: ");   
   makefile.write (a_node->getName ());
   makefile.newline ();
   makefile.write ("# Documento Configuracion:\n#   ");
   makefile.write (app.getConfigFile ());
   makefile.write (" version ");   
   makefile.writenl (description::functions::getVersion ());
   makefile.write ("# \n");
   
   makefile.debug (FILE_LOCATION);
   
   if (defines::getUseProjectConfig () == true) {
       makefile.write ("include $(BASEPJ)/");
       makefile.write (description::functions::getProjectConfig ());
       makefile.write ("\n\n");
   }
       
   makefile.writenl ("EXTERNAL_TEST=0");
   makefile.writenl ("VIEWER=@\n");
}

void generate::Structure::ending (generate::Makefile& makefile) 
   throw (RuntimeException)
{
   makefile.write ("FORCE:\n\n");
}

//------------------------------------------------------------------------------------------
// Invoca la creacion de los siguientes niveles de directorios.
//------------------------------------------------------------------------------------------
void generate::Structure::forward (const analysis::filesystem::Path* path)
   throw (RuntimeException)
{
   using namespace analysis::filesystem;
   
   LOGINFORMATION (
      string msg ("generate::Structure::forward | ");
      msg += a_node->asString ();
      Logger::information (msg, FILE_LOCATION);
   );
   
   const int mask = a_node->getRule ()->getMask ();
   
   Path::const_successor_iterator ii, maxii;
   const Node* child;
   AutoPointer prototype;
   const description::Rule* rule;
   
   for (ii = path->successor_begin (), maxii = path->successor_end (); ii != maxii; ii ++) {
      child = Path::successor (ii);

      if (child->isAPath () == false)
         continue;
         
      if ((rule = child->getRule ()) == NULL)
         continue;

      if ((rule->getType () & mask) != 0) {
         prototype = Agent::instance (this, child);
         prototype->execute ();         
         copy (prototype);
      }
   }      
}

//--------------------------------------------------------------------------------
// La clase que invoque a este metodo habra puesto el 
// "all : " y las reglas particulares que precedan a la lista de objetos
// dependientes del makefile que estamos creando.
//--------------------------------------------------------------------------------
void generate::Structure::writeAllDependences (generate::Makefile& makefile, generate::Prototype::dependence_container& container)
   throw (RuntimeException)
{
   dependence_iterator ii, maxii;
   int counter = 0;
   
   for (ii = container.begin (), maxii = container.end (); ii != maxii; ii ++) {
      if (++ counter > 2) {
         makefile.write (" \\\n\t");
         counter = 0;
      } 
      else
         makefile.space ();      
         
      makefile.write (dependence (ii)->getRelativePath (a_node));
   }   
}

void generate::Structure::writeClobber (Makefile& makefile) 
   throw (RuntimeException)
{
   makefile.debug (FILE_LOCATION);
   
   makefile.writenl ("clobber : FORCE");
   makefile.write ("\t@echo eliminado ");
   makefile.write (description::functions::getTargetDirectory ());
   makefile.writenl (" ...");
   
   makefile.write ("\t@find . -name \"");
   makefile.write (description::functions::getTargetDirectory ());
   makefile.writenl ("\" -exec rm -rf {} \\; -depth -print");   
   
   makefile.writenl ("\t@rm -f $(OBJECT_NAME)\n");
}


