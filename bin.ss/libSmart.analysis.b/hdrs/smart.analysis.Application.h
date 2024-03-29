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
#ifndef _smart_analysis_Application_h
#define _smart_analysis_Application_h

#include <nemesis.app.Application.h>

namespace smart {

namespace analysis {

namespace filesystem {
class Path;
}

using namespace nemesis;

class Application : public app::Application {
public:
   Application (const char *shortName, const char *title, const char *version);
   
   filesystem::Path* getcwd () throw () { return a_cwd; }
   const std::string& getConfigFile () const throw () { return a_configFile; }
   
protected:
   filesystem::Path* a_cwd;  

   void showTime (const Milisecond now) throw ();
   virtual void initialize () throw (RuntimeException);
   virtual void run () throw (RuntimeException);      
   
private:
   Milisecond a_initTime;
   std::string a_configFile;
   
   void getCurrentDirectory (char* cwd, const int maxsize) throw (RuntimeException);
};

}
}

#endif
