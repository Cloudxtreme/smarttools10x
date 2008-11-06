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

#include <iostream>
#include <algorithm>

#include <nemesis.CommandLine.h>

#include <smart.Variable.h>

#include <smart.structure.Agent.h>

#include <smart.ils.Target.h>
#include <smart.ils.Shuffler.h>

using namespace std;
using namespace nemesis;
using namespace smart;
using namespace smart::ils;

// enum Result { Done, Ignore };
enum Result { Ignore, Done };

//----------------------------------------------------------------------------------------
// Recive el nombre de directorio a comprobar. Usa el sistema de retorno habitual en los
// comandos de S.O. asi que devuelve 0 si hay que procesarlo o 1 en caso contrario. 
//----------------------------------------------------------------------------------------
int main (const int argc, const char* argv [])
{
   CommandLine& cl (CommandLine::instanciate ());
   Result result (Ignore);
   bool verbose = false;

   try {
      cl.add ("r", CommandLine::Argument::Mandatory, "Nombre del archivo que contiene los recursos");
      cl.add ("d", CommandLine::Argument::Mandatory, "Directorio del componente que va a crear");
      cl.add ("ce", CommandLine::Argument::Mandatory, "Extension del componente");
      cl.add ("n", CommandLine::Argument::Mandatory, "Nombre de real de la antigua variable TARGET");
      cl.add ("e", CommandLine::Argument::Mandatory, "Expression que describe el nombre del directorio destino (_{GMAKE_TARGET}.X p.e.)");
      cl.add ("v", CommandLine::Argument::Optional, "Activa el modo verbose", false);
      
      cl.initialize (argv, argc);
      cl.verify ();

      if (cl.exists ("v") == true)
         verbose = true;
      else
         smart::Variable::disableVerbose ();
      
      structure::Agent& structureAgent = structure::Agent::instance ();
      const structure::Section* root = structureAgent.uploadEnvironment ();
      
      string object (cl.getValue ("d"));
      string::size_type pos = object.find_last_of ('.');
      
      if (pos == string::npos) 
         object += cl.getValue ("ce");
      else 
         object.replace (pos, object.length () - pos, cl.getValue ("ce"));
      
      Target target (object.c_str (), cl.getValue ("n"), cl.getValue ("e"));      
      Shuffler shuffler (target);
            
      target.initialize (root);
      shuffler.initialize (cl.getValue ("r"));
            
      string candidate;
      for (Shuffler::iterator ii = shuffler.begin (), maxii = shuffler.end (); ii != maxii; ii ++) {
         candidate = shuffler.getTargetDirectory (ii);

         if (target.link (candidate) == true) {
            result = Done;
            break;        
         } 
      }      
   }
   catch (Exception& ex) {
      cerr << ex.asString () << endl;
   }   
   
   if (verbose == true)
      cout << "ILS: " << ((result == Done) ? "Done": "Ignore") << " (" << result << ')' << endl;
  
// siempre retorna cero, para indicar que ha terminado OK, por ahora no podemos actuar en el makefile en base
// al valor retornado por este ejecutable
   return 0;
}
