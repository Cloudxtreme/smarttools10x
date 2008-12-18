/*
   Smart.description - Version 10.
   Libreria para la descripcion de los elementos software con los que vamos a trabajar.

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
#include <nemesis.defines.h>
#include <nemesis.Logger.h>
#include <nemesis.functions.h>

#include <smart.naming.File.h>
#include <smart.naming.FileClass.h>

#include <smart.description.Rule.h>
#include <smart.description.RuleAgent.h>
#include <smart.description.Target.h>

using namespace std;
using namespace nemesis;
using namespace smart;

description::Rule::Type::Translate description::Rule::Type::st_translate [] = {
   { "FileSystem", description::Rule::Type::FileSystem },
   { "Executable", description::Rule::Type::Executable },
   { "Library", description::Rule::Type::Library },
   { "LogicalGroup", description::Rule::Type::LogicalGroup },
   { "HeaderRepository", description::Rule::Type::HeaderRepository },
   { "Header", description::Rule::Type::Header },
   { "AnsiC", description::Rule::Type::AnsiC },
   { "ASN1", description::Rule::Type::ASN1 },
   { "Plugin", description::Rule::Type::Plugin },
   { NULL, description::Rule::Type::Plugin }
};

description::Rule::Rule (const std::string& name, const Type::_v type) :
   a_name (name), 
   a_type (type),
   a_maxClass (0),
   a_maxTarget (0),
   a_generateHeader (false),
   a_mask (0),
   a_metaCommand (NULL)
{
}

void description::Rule::add (const naming::FileClass* fileClass) 
   throw (RuntimeException)
{
   if (fileClass == NULL)
      return;

   if (recognize (fileClass) == true)
      return;
      
   RuleAgent& rraa = RuleAgent::instance ();
   const Rule* other;
     
   if ((other = rraa.find (fileClass)) != NULL) {
      string msg (asString ());
      msg += " | ";
      msg += fileClass->asString ();
      msg += " | Asociada previamente a la regla: ";
      msg += other->asString ();
      throw RuntimeException (msg, FILE_LOCATION);
   }
             
   if (a_maxClass == MaxClass) {
      string msg ("smart::description::Rule::add  | ");
      msg += asString ();
      msg += functions::asText (" | Sobrepasada el numero maximo de ocurrencias (", MaxClass);
      msg += ')';
      throw RuntimeException (msg, FILE_LOCATION);
   }

   a_classes [a_maxClass ++] = fileClass;
}

void description::Rule::createTarget (const naming::FileClass* fileClass, const char* _suffix)
   throw (RuntimeException)
{
   if (fileClass == NULL)
      return;
      
   const Rule* other;
   
   if (a_maxTarget == MaxTarget) {
      string msg ("smart::description::Rule::createTarget  | ");
      msg += fileClass->asString ();
      msg += functions::asText (" | Sobrepasada el numero maximo de ocurrencias (", MaxTarget);
      msg += ')';
      throw RuntimeException (msg, FILE_LOCATION);
   }
   
   if ((other = RuleAgent::instance ().find (fileClass)) == NULL) {
      string msg (asString ());
      msg += " | ";
      msg += fileClass->asString ();
      msg += " | No esta asociada a ninguna regla";
      throw RuntimeException (msg, FILE_LOCATION);
   }

   LOGINFORMATION (
      string msg ("smart::description::Rule::createTarget | ");
      msg += asString ();
      msg += " | ";
      msg += fileClass->asString ();
      msg += " | Suffix: ";
      msg += (_suffix == NULL) ? "<null>": _suffix;
      Logger::information (msg, FILE_LOCATION);
   );   
               
   string suffix;
   
   if (_suffix != NULL) 
      suffix = _suffix;
      
   suffix += '.';

   a_targets [a_maxTarget ++] =  new Target (fileClass, suffix, other);
   
   if (a_generateHeader == false && other->getType () == Rule::Type::Header)
      a_generateHeader = true;
}

//-----------------------------------------------------------------------------------
// Obtiene el nombre final del .o que correspondera con el nombre recibido
// como parametro. No es tan simple como cambiar .x por .o ya que hay reglas que
// introduces prefijos. Por ejemplo kk.pcc -> kk.db.cc -> kk.db.o
//-----------------------------------------------------------------------------------
std::string description::Rule::getOutputName (const naming::File* _file) const 
   throw (RuntimeException)
{
   naming::File file (*_file);
   string result;
   
   const Rule* currentRule (this);
   const Target* target;
   bool stop = false;
   const_target_iterator ii, maxii;
   
   while (stop == false) {   
      stop = true;   
      for (ii = currentRule->target_begin (), maxii = currentRule->target_end (); ii != maxii; ii ++) {
         target = Rule::target (ii);

         if (target->getRule ()->isASource () == false)
            continue;

         currentRule = target->getRule ();
         target->rename (file);

         stop = false;
         break;
      }
   }
   
   result = file.getNameWithoutExtension ();
   result += ".o";   
   
   LOGDEBUG (
      string msg ("description::Rule::getOutputName | "); 
      msg += _file->asString ();
      msg += " | ";
      msg += asString ();
      msg += " | Result: ";
      msg += result;
      Logger::debug (msg, FILE_LOCATION);
   );

   return result;
}

std::string description::Rule::getAllOutputNames (const std::string& outputDirectory, const naming::File* _file) const 
   throw (RuntimeException)
{
   naming::File file (*_file);
   naming::File backup;
   string result;
   
   const Rule* currentRule (this);
   const Target* target;
   
   for (const_target_iterator ii = currentRule->target_begin (), maxii = currentRule->target_end (); ii != maxii; ii ++) {
      target = Rule::target (ii);

      if (target->getRule ()->isASource () == false)
         continue;

      currentRule = target->getRule ();
      backup = file;
      target->rename (file);
      
      result += outputDirectory;
      result += '/';
      result += file.getNameWithoutExtension ();
      result += ".o ";      
      file = backup;
   }
   
   LOGDEBUG (
      string msg ("description::Rule::getAllOutputNames | "); 
      msg += _file->asString ();
      msg += " | ";
      msg += asString ();
      msg += " | Result: ";
      msg += result;
      Logger::debug (msg, FILE_LOCATION);
   );
   
   return result;
}

void description::Rule::directAdd (const naming::FileClass* fileClass) 
   throw (RuntimeException)
{
   if (a_maxClass == MaxClass) {
      string msg ("smart::description::Rule::add  | ");
      msg += asString ();
      msg += functions::asText (" | Sobrepasada el numero maximo de ocurrencias (", MaxClass);
      msg += ')';
      throw RuntimeException (msg, FILE_LOCATION);
   }

   a_classes [a_maxClass ++] = fileClass;   
}

bool description::Rule::recognize (const naming::FileClass* fileClass) const
   throw ()
{
   if (fileClass != NULL) {
      for (register int i = 0; i < a_maxClass; i ++) {
         if (a_classes [i] == fileClass)
            return true;
      }
   }
     
   return false;   
}


string description::Rule::asString () const 
   throw ()
{
   string result ("smart::description::Rule { Name: ");
   result += a_name;
   result += " | Type: ";
   result += Type::asChar (a_type);   
   result += functions::asHexText (" (", a_type);
   result += functions::asHexText (") | Mask: ", a_mask);
   result += " | ClassFiles: ";
   
   if (a_maxClass == 0)
      result += "<none> ";
   else {
      for (int i = 0; i < a_maxClass; i ++) {
         result += a_classes [i]->getId ();
         result += ' ';
      }
   }
   
   return result += '}';   
}

description::Rule::Type::_v description::Rule::Type::asEnum (const char* str)
   throw (RuntimeException)
{
   _v result;
   int i;

   for (i = 0; st_translate [i].text != NULL; i ++) {
      if (strcasecmp (str, st_translate [i].text) == 0) {
         result = st_translate [i].type;
         break;
      }
   }
   
   if (st_translate [i].text  == NULL) {
      string msg ("Tipo de regla '");
      msg += str;
      msg += "' no reconocido";
      throw RuntimeException (msg, FILE_LOCATION);
   }
   
   return result;
}

const char* description::Rule::Type::asChar (const _v type) 
   throw ()
{
   for (int i = 0; st_translate [i].text != NULL; i ++) 
      if (type == st_translate [i].type)
         return st_translate [i].text;  
  
   return "<none>";
}
