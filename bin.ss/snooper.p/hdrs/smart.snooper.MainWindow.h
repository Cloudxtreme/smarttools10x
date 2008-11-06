/*
   snooper - Version 10.
   Herramienta que establece las variables de entorno en base a la configuracion establecida por el usuario
   mediante la seleccion por los menus. Los menus son creados dinamicamente en base ala informacion contenida
   en el directorio de configuraciones.

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
#ifndef _smart_snooper_MainWindow_h
#define _smart_snooper_MainWindow_h

#include <QMainWindow>

#include <nemesis.RuntimeException.h>
#include <nemesis.SortedVector.h>

#include <smart.snooper.MakefileVariable.h>

// Clases obtenidas a partir de un FORM del QT designer
namespace Ui {
class MainWindow;
}

class QListWidgetItem;

namespace smart {

namespace structure {
class Section;
}

namespace snooper {

using namespace nemesis;

class MainWindow : public QMainWindow {
Q_OBJECT
   
public: 
   typedef SortedVector <MakefileVariable, MakefileVariable::SortByName, const std::string&> makefile_variable_container;
   typedef makefile_variable_container::iterator makefile_variable_iterator;
   
   MainWindow (QWidget* parent = 0);
   
   void initialize () throw (RuntimeException);

   MakefileVariable* find (const char* name) throw () {
      std::string aux (name);
      return  a_makefileVariables.find (aux);
   }
      
public slots:
   void changeEnvironVariable (QListWidgetItem*,QListWidgetItem*);
   void changeMakefileVariable (QListWidgetItem*,QListWidgetItem*);
   void about ();
   
private:
   Ui::MainWindow* a_gui;
   makefile_variable_container a_makefileVariables;
   
   void forward (const structure::Section* root, const structure::Section*) throw (RuntimeException);   
   void interpret (const std::string& fullpath) throw (RuntimeException);   
};

}
}

#endif
