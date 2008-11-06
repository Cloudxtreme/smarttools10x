/*
   configure - Version 10.
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
#include <iostream>

#include <sys/stat.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

#include <nemesis.defines.h>
#include <nemesis.Logger.h>
#include <nemesis.CommandLine.h>
#include <nemesis.TraceWriter.h>
#include <nemesis.Tokenizer.h>

#include <nemesis.xml.DocumentFile.h>

#include <nemesis.io.functions.h>

#include <smart.sccs.h>
#include <smart.functions.h>
#include <smart.Variable.h>

#include <smart.configure.sccs.h>
#include <smart.configure.Application.h>
#include <smart.configure.Outline.h>
#include <smart.configure.Resource.h>
#include <smart.configure.Tuning.h>

#include <smart.description.RuleAgent.h>
#include <smart.description.functions.h>

using namespace std;
using namespace nemesis;
using namespace smart;

smart_import_sccs (Configure)

configure::Application::Application () : 
   app::Application ("SmartTools.Configure", "Configurador del sistema", smart_use_sccs_version (Configure), __DATE__, __TIME__),
   a_outline (Directory::Mode::Normal),
   a_ostream (-1),
   a_tuning (NULL)
{
   configure::sccs::activate ();

   CommandLine& ccll (CommandLine::instanciate ());
      
   ccll.add ("d", CommandLine::Argument::Optional, "Documento XML de configuracion");
   ccll.add ("c", CommandLine::Argument::Optional, "Ruta a las configuraciones");
   ccll.add ("t", CommandLine::Argument::Optional, "Nivel de trazas");

   activateGeneralPublicLicense ();
}

void configure::Application::initialize () 
   throw (RuntimeException)
{
   CommandLine& ccll (CommandLine::instanciate ());

   if (ccll.exists ("t") == true) 
      Logger::setLevel (Logger::asLevel (ccll.getValue ("t")));

   Logger::initialize ("configure", new TraceWriter (".configure.trace", 4096000));
}

void configure::Application::run () 
   throw (RuntimeException)
{
   CommandLine& ccll (CommandLine::instanciate ());   

   string dname;

   if (ccll.exists ("c") == true)
      dname = ccll.getValue ("c");
   else {
      Variable home ("GMAKE_HOME");      
      dname = home.getValue ();
      dname += "/data";
      cout << endl;
   }
      
   string osName (smart::functions::getOSName ());
   string archName (smart::functions::getArchitecture ());

   dname += '/';
   dname += osName;
   dname += '/';
   dname += archName;   
   
   cout << "Cargando guion de configuracion : " << dname << endl << endl;

   static const int omode = O_WRONLY | O_CREAT | O_TRUNC;
   static const int mask =  S_IRUSR | S_IWUSR | S_IXUSR;
   
   a_target = functions::getSystemId ();

   string autoName (".");
   autoName += a_target;
   autoName += ".auto";
   
   nemesis_signal_shield (a_ostream, open (autoName.c_str (), omode, mask));

   if (a_ostream == -1)
      throw RuntimeException (autoName, errno, FILE_LOCATION);

   LOGINFORMATION (
      string msg ("Output filename: ");
      msg += autoName;
      Logger::information (msg, FILE_LOCATION);
   );

   write (NULL, "GMAKE_CONFIG", dname);
   write (NULL, "GMAKE_OSNAME", osName);
   write (NULL, "GMAKE_PROCESSORTYPE", archName);
   write (NULL, "GMAKE_SYSTEM", a_target);
   write (NULL, "GMAKE_EOL", "' '");
   
   xml::DocumentFile configuration;
   
   if (ccll.exists ("d") == true) 
      configuration.initialize (ccll.getValue ("d"));
   else {
      Variable home ("GMAKE_HOME");      
      string config_xml = home.getValue ();
      config_xml += "/data/config.xml";
      configuration.initialize (config_xml.c_str ());
      cout << endl;
   }
   
   description::RuleAgent::instance ().config (configuration); 

   Variable mypj ("MYPJ");
   string system_tuning (mypj.getValue ());
   system_tuning += '/';
   system_tuning += a_target; 
   system_tuning += ".tuning";
   
   cout << "Usando " << system_tuning << " ... " << flush;
   
   if (io::functions::exists (system_tuning) == true) {
      a_tuning = new Tuning;
      a_tuning->initialize (system_tuning.c_str ());
      cout << "Ok" << endl << endl;
   }
   else
      cout << "No" << endl << endl;
      
   /*
    * La variable a_target comienza con el nombre del GMAKE_SYSTEM
    * y se va ampliando cada vez que desde la ejecución del guión se
    * llama al Application::write
    */      
   try {
      a_baseTarget = a_target;
      a_outline.initialize (dname);
      a_outline.run (); 
      
      // a_target tiene el contenido real del GMAKE_TARGET
      // a_baseTarget contiene la expresión, pero con todos los recursos que pueden cambiar
      // puestos con valor 'none'.
      dname = description::functions::getTargetName ();
      write (NULL, dname, a_target);
      
      write (NULL, "GMAKE_BASETARGET", a_baseTarget);
      
      dname += "_DIR";
      a_target = description::functions::getTargetPrefix ();
      a_target += "${";
      a_target += description::functions::getTargetName ();
      a_target += '}';
      a_target += description::functions::getTargetExtension ();
      write (NULL, dname, a_target);
       
      a_target = "\"config ";
      a_target += a_outline.serialize ();
      a_target += "\"";
      write (NULL, "GMAKE_STRUCTURE", a_target);
        
      close (a_ostream);

      cerr << autoName << endl << endl;
      cout << endl;
   }
   catch (RuntimeException&) {      
      close (a_ostream);
      a_ostream = -1;
      throw;
   }
}

void configure::Application::write (const configure::Resource* resource, const string& variable, const string& value)
   throw (RuntimeException)
{
   int r;

   string line ("export ");
   line += variable;
   line += '=';
   line += value;
   line += "\n";

   const char* _value = line.c_str ();

   nemesis_signal_shield (r, ::write (a_ostream, _value, nemesis_strlen (_value)));

   if (r < 0)
      throw RuntimeException (line, errno, FILE_LOCATION);
      
   if (resource != NULL && resource->onTarget () == true) {
      a_target += '_';
      a_baseTarget += '_';
      
      if (value.find ("/") != string::npos) {
         char* init;
         char* aux; 
         const char* src = value.c_str ();
         
         aux = init = new char [value.length ()]; 
         
         while (*src != 0) {
            if (*src != '/')
               *aux ++ = *src;
            src ++;
         }
         *aux = 0;
         a_target += init;         
         a_baseTarget += (resource->enableILS () == true) ? "none": init;
         
         delete [] init;
      }
      else {
         if (resource->enableILS () == true)
            a_baseTarget += "none";
         else
            a_baseTarget += value;
         
         a_target += value;
      }
   }
   
   LOGINFORMATION (
      string msg ("smart::configure::Application::write | ");
      msg += line;
      Logger::information (msg, FILE_LOCATION);
   );      
}

