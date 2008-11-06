/*
   Smart.structure - Version 10.
   Libreria para gestionar la estructura de resource/vendor/version implicados a la hora de 
   la generacion de ejecutables. 

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
#ifndef _smart_structure_Agent_h
#define _smart_structure_Agent_h

#include <vector>

#include <nemesis.RuntimeException.h>
#include <nemesis.Singleton.h>

namespace smart {

class Section;

namespace structure {

class Section;
class Resource;

using namespace nemesis;

class Agent : public Singleton <Agent> {
public:
   const Section* uploadEnvironment () throw (RuntimeException);

private:
   Section* a_root;
   
   Agent () : a_root (NULL) {;}
   
   static void forward (Section&, const std::string& resources) throw (RuntimeException);

   friend class Singleton <Agent>;
};

}
}

#endif
