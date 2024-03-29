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
#include <nemesis.ModuleManager.h>

#include <smart.sccs.h>
#include <smart.structure.sccs.h>

smart_define_sccs_tag (structure, 0) 

void smart::structure::sccs::activate ()
   throw ()
{
   smart::sccs::activate ();
   nemesis::ModuleManager::instanciate ().insert (smart_use_sccs_tag (structure), "00");
}

