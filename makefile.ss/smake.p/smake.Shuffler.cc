/*
   smake - Version 10.0

   Desarrollado por la division de Nuevos conceptos de Plataformas.
   (c) Copyright 2005-2008 Telefonica Investigacion y Desarrollo. S.A. Unipersonal.

   SmartMake, genera los objetos de librer�as y ejecutables contenidos en un 
   proyecto determinado. Combina los distintos recursos disponibles para intentar
   optimizar el rendimiento mediante el ILS.

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
#include <sys/stat.h>
#include <stdlib.h>

#include <nemesis.defines.h>
#include <nemesis.Tokenizer.h>
#include <nemesis.Logger.h>

#include <nemesis.io.TextReader.h>

#include <smart.functions.h>

#include <smart.description.functions.h>

#include <smart.structure.Resource.h>

#include <smart.smake.Shuffler.h>
#include <smart.smake.Target.h>

using namespace std;
using namespace nemesis;
using namespace smart;

//----------------------------------------------------------------------------------------------------
// Establece la lista de variables que son subsceptibles de poder ser cambiadas para optimizar
// la creacion la libreria actual y hacer el link otra ya existente.
//
// La lista estara formada por los recursos que pueden ser empleados en el ILS y que no estan
// referenciados en el (makefile.resources).
//
// (1) Si el recurso yo vale "none" no tiene que entrar en la combinatoria
//----------------------------------------------------------------------------------------------------
void smake::Shuffler::initialize (const char* makefile_resources) 
   throw (RuntimeException)
{
   loadRequests (a_requests, makefile_resources);

   const structure::Resource* resource;
   
   request_iterator rr, maxrr = request_end ();

   for (const_resource_iterator ii = a_target.resource_begin (), maxii = a_target.resource_end (); ii != maxii; ii ++) {
      resource = ResourceContainer::resource (ii);
      
      if (resource->enableILS () && resource->getVariable ()->getValue () != "none") {       // (1)
         if ((rr = request_find (resource->getVariable ()->getName ())) == maxrr)
            add (resource);
         else if (request_value (rr) == "none")
            add (resource);
      }
   }
}

smake::Shuffler::request_iterator smake::Shuffler::request_find (const string& name)
   throw ()
{
   request_iterator ii, maxii;

   for (ii = request_begin (), maxii = request_end (); ii != maxii; ii ++) {
      if (request_name (ii) == name)
         break;
   }
   
   return ii;
}

//-----------------------------------------------------------------------------------------------
// (0) El primer componente de TARGET no es el contenido del GMAKE_CONFIGURE sino la
//     contatenacion de SO + arquitectura de la maquina.
// (1) Puede ser que el recurso pueda ser usado para el ILS pero que este desactivado por 
//     estar referenciado en el (makefile.resources).
// (2) Al hacer las combinaciones las variables pierden sus valores originales, con lo que en las
//    siguientes vueltas hay que volver a reponerlo. Por ejemplo GMAKE_RDBMS comienza valiendo oracle9, 
//    al hacer la primera combinacion se poner a "none", la siquiente combinacion deberia recuperar 
//    su valor original y pasar al siguiente elemento.
// (3) Verifica que no esta cambiando un recurso sobre el que hay impuesta una condicion 
//     determinada.
//-----------------------------------------------------------------------------------------------
bool smake::Shuffler::prepareEnvironment (const smake::Shuffler::iterator& index)
   throw (RuntimeException)
{
   string value;
   bool first (true);
      
   const_resource_iterator jj, maxjj = resource_end ();
   const structure::Resource* resource;
   int counter;
   bool keepValue;
   const char* newValue (NULL);

   for (const_resource_iterator ii = a_target.resource_begin (), maxii = a_target.resource_end (); ii != maxii; ii ++) {
      resource = ResourceContainer::resource (ii);
      
      if (first == false)
         value += '_';
      else {
         value = functions::getSystemId ();                                      // (0)
         first = false;
         continue;
      }
      
      if (resource->enableILS () == false) {
         value += resource->getVariable ()->getValue ();
         continue;
      }

      keepValue = true;                                                                   // (1)
            
      for (jj = resource_begin (), counter = 0; jj != maxjj; jj ++, counter ++) {      
         if (resource == ResourceContainer::resource (jj)) {
            if (((1 << counter) & index) == 0)
               keepValue = false;     
            break;
         }
      }

      const string& variableName = resource->getVariable ()->getName ();
      
      if (keepValue == true) {
         const std::string& rv = resource->getVariable ()->getValue ();                   // (2)
         
         request_iterator ii = request_find (variableName);
      
         if (ii != request_end ()) {
            if (match (variableName, request_value (ii), rv) == false)                    // (3)
               return false;
         }
         
         newValue = rv.c_str ();
      }
      else 
         newValue = "none";
      
      setEnvironment (variableName, newValue);
      value += newValue;
   }
   
   string::size_type pos;
   
   while ((pos = value.find ('/')) != string::npos) 
      value.erase (pos, 1);   

   setEnvironment (a_target.getName (), value.c_str ());
   
   string gmakeTargetDir (description::functions::getTargetPrefix ());
   gmakeTargetDir += value;
   gmakeTargetDir += description::functions::getTargetExtension ();
   
   string name (description::functions::getTargetName ());
   name += "_DIR";
   
   setEnvironment (name, gmakeTargetDir.c_str ());
   
   return true;
}

void smake::Shuffler::setEnvironment (const string& variable, const char* value)
   throw (RuntimeException)
{
   static std::string aux;

   LOGINFORMATION (
      string msg ("smake::Shuffler::setEnvironment | Variable: ");
      msg += variable;
      msg += " | Value: ";
      msg += value;
      Logger::information (msg, FILE_LOCATION);
   );      

   int len = variable.length () + nemesis_strlen (value) + 2;
   char* env = new char [len];
   
   nemesis_strcat (nemesis_strcat (nemesis_strcpy (env, variable.c_str ()), "="), value);
   
   if (putenv (env) !=0) {
      string msg ("Error al establecer ");
      msg += env;
      throw RuntimeException (msg, FILE_LOCATION);
   }
}
   
//-----------------------------------------------------------------------------------------------
// Si existe cargar el fichero (makefile.resources) ya que los valores de las variables
// incluidas en este, no entraran en las combinaciones que cambian a 'none'.
//-----------------------------------------------------------------------------------------------
void smake::Shuffler::loadRequests (smake::Shuffler::request_container& requests, const char* makefile_resources)
   throw (RuntimeException)
{
   struct stat statData;
   bool result (true);
      
   if (stat (makefile_resources, &statData) == 0) {
      io::TextReader reader;
      const char* line;
      Tokenizer tk;

      reader.open (makefile_resources);

      tk.activateStrip (true);

      while ((line = reader.fetch ()) != NULL) {
         tk.apply (line, ":");

         if (tk.size () != 2) 
            continue;
            
         requests.push_back (request (tk [0], tk [1]));
      }
   }   
}

/*
 * Hay que tener en cuenta que la línea del fichero de recursos permite las condiciones OR
 * Así la línea GMAKE_RDBMS: oracle mysql => indicaría que el componente puede crearse
 * cuando la configuración establecida incluya información sbore cualiera de los dos RDBMSs
 */ 
bool smake::Shuffler::match (const string& name, const string& reqValue, const string& value)
   throw ()
{
   bool result (false);
   const int nv = value.length ();
   const char* v = value.c_str ();      
   const char* r = reqValue.c_str ();
   int nr;
   
   if (nemesis_strchr (r, ' ') == 0) {
      if ((nr = reqValue.length ()) <= nv)
         result = ((nr == nv) ? nemesis_strcmp (v, r): strncmp (v, r, nr)) == 0;      
   }
   else {
      Tokenizer multivalue (r, " ");
      
      multivalue.activateStrip (true);
      
      for (Tokenizer::const_iterator ii = multivalue.begin (), maxii = multivalue.end (); ii != maxii && result == false; ii ++) {
         const std::string& token = Tokenizer::data (ii);
         r = token.c_str ();
         
         if ((nr = nemesis_strlen (r)) <= nv)  
            result = ((nr == nv) ? nemesis_strcmp (v, r): strncmp (v, r, nr)) == 0;            
      }
   }
   
   LOGINFORMATION (   
      string msg ("smake::Shuffler::match | Variable: ");
      msg += name;
      msg += " | Value: ";
      msg += value;
      msg += " | Required: ";
      msg += reqValue;
      msg += " | Result: ";
      msg += nemesis::functions::asString (result);
      Logger::information (msg, FILE_LOCATION);
   );      
   
   return result;
}

