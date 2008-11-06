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
#ifndef _smart_generate_AutoPointer_h
#define _smart_generate_AutoPointer_h

#include <smart.generate.Agent.h>

namespace smart {

namespace generate {

class Prototype;

class AutoPointer {
public:
   explicit AutoPointer (Prototype* t) : a_prototype (t) {;}   
   AutoPointer () : a_prototype (NULL) {;}
   
   ~AutoPointer () { if (a_prototype != NULL) Agent::release (a_prototype); }

   Prototype* operator -> () const throw () { return a_prototype; }
   
   Prototype* operator = (Prototype* t) throw () { 
      if (a_prototype != t) {
         Agent::release (a_prototype);          
         a_prototype = t; 
      }
      return a_prototype;
   }

   operator Prototype* () const throw () { return a_prototype; }
   
private:
   Prototype* a_prototype;   
};


}
}



#endif


