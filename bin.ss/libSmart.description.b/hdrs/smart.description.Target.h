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
#ifndef _smart_description_Target_h
#define _smart_description_Target_h

namespace smart {

namespace naming {
class File;
class FileClass;
}

namespace description {

class Rule;

using namespace nemesis;

class Target {
public:
   void rename (naming::File&) const throw ();
   
//   const naming::FileClass* getFileClass () const throw () { return a_fileClass; }
  // const std::string& getSuffix () const throw () { return a_suffix; }
   const Rule* getRule () const throw () { return a_rule; }

private:
   const naming::FileClass* a_fileClass;
   std::string a_suffix;
   const Rule* a_rule;
   
   Target (const naming::FileClass* fileClass, const std::string& suffix, const Rule* rule) :
      a_fileClass (fileClass), a_suffix (suffix), a_rule (rule)
   {;}

   friend class Rule;
};

}
}

#endif
