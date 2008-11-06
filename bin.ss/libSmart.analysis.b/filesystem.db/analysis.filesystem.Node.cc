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
#include <nemesis.Logger.h>

#include <smart.analysis.FileSystem.h>
#include <smart.analysis.filesystem.Path.h>
#include <smart.analysis.filesystem.FlatEntry.h>

using namespace std;
using namespace nemesis;
using namespace smart;

//-------------------------------------------------------------------------------------------
// Recupera el primer nivel de dependencias logicas. No se puede recuperar todo el arbol
// de dependencias porque es posible que haya nodos que todavia estan en proceso de estudio
// y podriamos meternos en bucles infinitos. 
//
// Asi que en la primera pasada saca las dependencias directas, y una vez que todos los 
// nodos estan preparados se vuelve a hacer otra pasa para sacar todo el esquema de dependencias
// de cada uno delos nodos.
//-------------------------------------------------------------------------------------------
void analysis::filesystem::Node::interpret () 
   throw (RuntimeException)
{
   if (isInterpreted () == true)
      return;

   LOGINFORMATION (
      string msg ("smart::analysis::filesystem::Node::interpret | ");
      msg += asString ();
      Logger::information (msg, FILE_LOCATION);
   );
   
   if (begin () == false)
      return;   

   try {
      do_interpret ();
      end ();
   }
   catch (RuntimeException&) {
      end ();
      throw;
   }
   
   interpreted ();
}

//-------------------------------------------------------------------------------------------
// Recupera el primer nivel de dependencias logicas. No se puede recuperar todo el arbol
// de dependencias porque es posible que haya nodos que todavia estan en proceso de estudio
// y podriamos meternos en bucles infinitos. 
//
// Asi que en la primera pasada saca las dependencias directas, y una vez que todos los 
// nodos estan preparados se vuelve a hacer otra pasa para sacar todo el esquema de dependencias
// de cada uno delos nodos.
//-------------------------------------------------------------------------------------------
void analysis::filesystem::Node::synthesize () 
   throw (RuntimeException)
{
   if (isSynthesized () == true)
      return;

   LOGINFORMATION (
      string msg ("smart::analysis::filesystem::Node::synthesize | ");
      msg += asString ();
      Logger::information (msg, FILE_LOCATION);
   );
   
   if (begin () == false)
      return;   

   try {
      do_synthesize ();
      end ();
   }
   catch (RuntimeException& ex) {
      end ();
      throw;
   }

   synthesized ();
}

