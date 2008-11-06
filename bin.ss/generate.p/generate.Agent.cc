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

#include <smart.naming.FileClass.h>

#include <smart.analysis.filesystem.Node.h>

#include <smart.description.RuleAgent.h>
#include <smart.description.Rule.h>

#include <smart.generate.Agent.h>
#include <smart.generate.FileSystem.h>
#include <smart.generate.LogicalGroup.h>
#include <smart.generate.Library.h>
#include <smart.generate.Executable.h>
#include <smart.generate.Command.h>

using namespace std;
using namespace nemesis;
using namespace smart;

generate::Agent::filesystem_container generate::Agent::st_filesystems;
generate::Agent::logicalgroup_container generate::Agent::st_logicalgroups;
generate::Agent::library_container generate::Agent::st_libraries;
generate::Agent::executable_container generate::Agent::st_executables;
generate::Agent::command_container generate::Agent::st_commands;

generate::Prototype* generate::Agent::instance (generate::Prototype* predecessor, const analysis::filesystem::Node* node) 
   throw (RuntimeException)
{
   using namespace description;
   
   generate::Prototype* result (NULL);
   
   RuleAgent& ruleAgent (RuleAgent::instance ());
   const Rule* rule = ruleAgent.find (node->getFileClass ());
   
   LOGINFORMATION (
      string msg ("generate::Agent::instance | ");
      msg += node->asString ();
      Logger::information (msg, FILE_LOCATION);
   );
   
   if (rule == NULL) {
      string msg (node->asString ());
      msg += " | No tiene regla asociada";
      throw RuntimeException (msg, FILE_LOCATION);
   }

   switch (rule->getType ()) {
      case Rule::Type::FileSystem: result = st_filesystems.create (); break;      
      case Rule::Type::LogicalGroup: result = st_logicalgroups.create (); break;      
      case Rule::Type::Library: result = st_libraries.create (); break;      
      case Rule::Type::Executable: result = st_executables.create (); break;      
      default: result = st_commands.create (); break;
   }      
   
   if (result == NULL) {
      string msg (node->asString ());
      msg += " | Nodo no tiene asociado ningun generador";
      throw RuntimeException (msg, FILE_LOCATION);      
   }
   
   result->initialize (predecessor, node);
   
   return result;
}

void generate::Agent::release (generate::Prototype* prototype) 
   throw ()
{
   if (prototype == NULL)
      return;

   prototype->clear ();
   
   switch (prototype->a_type) {
      case Prototype::Type::FileSystem:
         st_filesystems.release (static_cast <FileSystem*> (prototype));
         break;
      case Prototype::Type::LogicalGroup:
         st_logicalgroups.release (static_cast <LogicalGroup*> (prototype));
         break;
      case Prototype::Type::Library:
         st_libraries.release (static_cast <Library*> (prototype));
         break;
      case Prototype::Type::Executable:
         st_executables.release (static_cast <Executable*> (prototype));
         break;
      case Prototype::Type::Command:
         st_commands.release (static_cast <Command*> (prototype));
         break;
   }      
}

