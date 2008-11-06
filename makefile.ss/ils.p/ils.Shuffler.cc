/*
   ILS - Version 10.0

   Desarrollado por la division de Nuevos conceptos de Plataformas.
   (c) Copyright 2005-2008 Telefonica Investigacion y Desarrollo. S.A. Unipersonal.

   Intelligent Linkage System 

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

#include <nemesis.defines.h>
#include <nemesis.Tokenizer.h>

#include <nemesis.io.TextReader.h>

#include <smart.functions.h>

#include <smart.structure.Resource.h>

#include <smart.ils.Shuffler.h>
#include <smart.ils.Target.h>

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
void ils::Shuffler::initialize (const char* makefile_resource) 
   throw (RuntimeException)
{
   request_container requests;
   
   loadRequests (requests, makefile_resource);

   const structure::Resource* resource;
   
   request_iterator end = requests.end ();

   for (const_resource_iterator ii = a_target.resource_begin (), maxii = a_target.resource_end (); ii != maxii; ii ++) {
      resource = ResourceContainer::resource (ii);
      
      if (resource->enableILS () && resource->getVariable ()->getValue () != "none") {       // (1)
         request_iterator ii = find (requests.begin (), end, resource->getVariable ()->getName ());
         
         if (ii == end)
            add (resource);
      }
   }
}

//-----------------------------------------------------------------------------------------------
// (0) El primer componente de TARGET no es el contenido del GMAKE_CONFIGURE sino la
//     contatenacion de SO + arquitectura de la maquina.
// (1) Puede ser que el recurso pueda ser usado para el ILS pero que este desactivado por 
//     estar referenciado en el (makefile.resources).
//-----------------------------------------------------------------------------------------------
string ils::Shuffler::getTargetDirectory (const ils::Shuffler::iterator& index)
   throw (RuntimeException)
{
   string value;
   bool first (true);
      
   const_resource_iterator jj, maxjj = resource_end ();
   const structure::Resource* resource;
   int counter;
   bool keepValue;

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
      
      if (keepValue == true)
         value += resource->getVariable ()->getValue ();
      else
         value += "none";
   }
   
   string::size_type pos;
   
   while ((pos = value.find ('/')) != string::npos) 
      value.erase (pos, 1);   

   return a_target.calculeDirectory (value);
}


//-----------------------------------------------------------------------------------------------
// Si existe cargar el fichero (makefile.resources) ya que los valores de las variables
// incluidas en este, no entraran en las combinaciones que cambian a 'none'.
//-----------------------------------------------------------------------------------------------
void ils::Shuffler::loadRequests (ils::Shuffler::request_container& requests, const char* makefile_resource)
   throw (RuntimeException)
{
   struct stat statData;
   bool result (true);
      
   if (stat (makefile_resource, &statData) == 0) {
      io::TextReader reader;
      const char* line;
      Tokenizer tk;

      reader.open (makefile_resource);

      tk.activateStrip (true);

      while ((line = reader.fetch ()) != NULL) {
         tk.apply (line, ":");

         if (tk.size () != 2) 
            continue;
            
         requests.push_back (tk [0]);
      }
   }   
}

