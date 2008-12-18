/*
   Smart.core - Core de las aplicaciones Smart. 
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
   
#ifndef _smart_sccs_h
#define _smart_sccs_h

#ifndef _MT
   #ifndef _DEBUG
      #define smart_define_sccs_tag(module,release)  \
         const char* smart_sccs_##module = "@(#)SmartTools."#module" VERSION 11.0."#release"/Evo-X/ST/O"; \
         const char* smart_version_##module = "11.0."#release"/Evo-X";
   #else
      #define smart_define_sccs_tag(module,release)  \
         const char* smart_sccs_##module = "@(#)SmartTools."#module" VERSION 11.0."#release"/Evo-X/ST/D"; \
         const char* smart_version_##module = "11.0."#release"/Evo-X";
   #endif
#else
   #ifndef _DEBUG
      #define smart_define_sccs_tag(module,release)  \
         const char* smart_sccs_##module = "@(#)SmartTools."#module" VERSION 11.0."#release"/Evo-X/MT/O"; \
         const char* smart_version_##module = "11.0."#release"/Evo-X";
   #else
      #define smart_define_sccs_tag(module,release)  \
         const char* smart_sccs_##module = "@(#)SmartTools."#module" VERSION 11.0."#release"/Evo-X/MT/D"; \
         const char* smart_version_##module = "11.0."#release"/Evo-X";
   #endif
#endif

#define smart_import_sccs(module) \
   extern const char* smart_sccs_##module; \
   extern const char* smart_version_##module;

#define smart_use_sccs_tag(module) (const char *) smart_sccs_##module
#define smart_use_sccs_version(module) (const char *)  smart_version_##module

namespace smart {

class sccs {
public:  
   static void activate () throw ();
};

}

#endif

