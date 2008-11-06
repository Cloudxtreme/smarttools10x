/*
   smake - Version 10.0

   Desarrollado por la division de Nuevos conceptos de Plataformas.
   (c) Copyright 2005-2008 Telefonica Investigacion y Desarrollo. S.A. Unipersonal.

   SmartMake, genera los objetos de librerías y ejecutables contenidos en un 
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

/*
   smake - Version 10.
   Herramienta que genera las combinaciones de TARGET's precisas para optimizar el rendimiento del ILS.

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
#ifndef _smart_smake_Target_h
#define _smart_smake_Target_h

#include <vector>

#include <nemesis.defines.h>
#include <nemesis.RuntimeException.h>

#include <smart.Variable.h>

#include <smart.smake.ResourceContainer.h>

namespace smart {

namespace smake {

using namespace nemesis;

class Target : public ResourceContainer {
public:
   explicit Target (const std::string& targetName, const std::string& expression);
      
   void initialize (const structure::Section*) throw (RuntimeException);
   void compile (const std::string& candidate) throw ();

   const std::string& getName () const throw () { return a_targetName; }
   const std::string& getValue () const throw (RuntimeException) { return a_gmakeTarget.getValue (); }
   
   std::string calculeDirectory (const std::string& targetValue) const throw (RuntimeException);
   
private:
   const std::string& a_targetName;
   const std::string& a_expression;
   Variable a_gmakeTarget;
};

}
}

#endif

