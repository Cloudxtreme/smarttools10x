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
#ifndef _smart_description_functions_h
#define _smart_description_functions_h

#include <nemesis.functions.h>

namespace nemesis {
namespace xml {
   class Node;
}
}

namespace smart {

namespace description {

using namespace nemesis;

class functions : public nemesis::functions {
public:  
   static void setup (const xml::Node*) throw (RuntimeException);
   
   // Esta se usa con un forma especial $(XX) para los Makefiles, que no servir para 
   // definir variables de entorno que tendria que ser $XX o ${XX}
   static const std::string& getTargetDirectory () throw () { return st_targetDirectory; }
   static const std::string& getTargetPrefix () throw () { return st_targetPrefix; }
   static const std::string& getTargetName () throw () { return st_targetName; }
   static const std::string& getTargetExtension () throw () { return st_targetExtension; }
   static const std::string& getProjectConfig () throw () { return st_projectConfig; }
   static const std::string& getTuneProjectConfig () throw () { return st_tuneProjectConfig; }
   static const std::string& getMakeName () throw () { return st_makeName; }
   static const std::string& getMakeConfig () throw () { return st_makeConfig; }
   static const std::string& getMakeResource () throw () { return st_makeResource; }
   static const std::string& getVersion () throw () { return st_version; }

private:
   static std::string st_targetDirectory;
   static std::string st_targetPrefix;
   static std::string st_targetName;
   static std::string st_targetExtension;
   static std::string st_projectConfig;
   static std::string st_tuneProjectConfig;
   static std::string st_makeName;
   static std::string st_makeConfig;
   static std::string st_makeResource;
   static std::string st_version;
};

}
}

#endif

