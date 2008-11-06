#include <iostream>
#include <functional>

#include <nemesis.Logger.h>
#include <nemesis.Exception.h>
#include <nemesis.functions.h>
#include <nemesis.Thread.h>
#include <nemesis.ThreadManager.h>
#include <nemesis.Tokenizer.h>

#include <nemesis.CommandLine.h>

#include <nemesis.io.sccs.h>

#include <smart.onlinehelp.sccs.h>
#include <smart.onlinehelp.Application.h>
#include <smart.onlinehelp.Document.h>
#include <smart.onlinehelp.Frame.h>
#include <smart.onlinehelp.LibraryKdoc.h>
#include <smart.onlinehelp.LibraryDoxygen.h>

smart_import_sccs (OnlineHelp)

using namespace std;
using namespace nemesis;
using namespace smart;

onlinehelp::Application::Application () :
   app::Application ("onlinehelp", "Compilacion automatica de documentacion on-line", smart_use_sccs_version (OnlineHelp), __DATE__, __TIME__),
   a_responseCounter (0)
{
   using namespace nemesis::app;

   CommandLine& commandLine (CommandLine::instanciate ());

   io::sccs::activate ();
   onlinehelp::sccs::activate ();

   commandLine.add ("ss", CommandLine::Argument::Mandatory, "Nombre del subsistema");
   commandLine.add ("l", CommandLine::Argument::Mandatory, "Lista de directorios separados por comas sin espacios a procesar");
   commandLine.add ("o", CommandLine::Argument::Mandatory, "Pagina en HTML usada como resumen");
   commandLine.add ("t", CommandLine::Argument::Optional, "Titulo de la pagina");
   commandLine.add ("kdoc", CommandLine::Argument::Optional, "Procesar informacion generada por kdoc", false);
   commandLine.add ("doxygen", CommandLine::Argument::Optional, "Procesar informacion generada por doxygen", false);
}

void onlinehelp::Application::initialize ()
   throw (RuntimeException)
{
   CommandLine& commandLine (CommandLine::instanciate ());
   const Tokenizer libs (commandLine.getValue ("l"), ",");
   string libraryName;
   
   if (commandLine.exists ("kdoc") == true) {
      cout << "Cargado modulo de analisis de kdoc ..." << endl;
      for (int i = 0, maxi = libs.size (); i < maxi; i ++) 
         a_libraries.push_back (new LibraryKdoc (libs [i]));
   }
   else if (commandLine.exists ("doxygen") == true) {
      cout << "Cargado modulo de analisis de doxygen ..." << endl;
      for (int i = 0, maxi = libs.size (); i < maxi; i ++) 
         a_libraries.push_back (new LibraryDoxygen (libs [i]));
   }
   else
      throw RuntimeException ("Hay que indicar el modulo de analisis", FILE_LOCATION);
}

void onlinehelp::Application::run ()
   throw (RuntimeException)
{
   CommandLine& commandLine (CommandLine::instanciate ());

   string subsystemName (commandLine.getValue ("ss"));
   string overview (commandLine.getValue ("o"));

   Document libraries (Document::Libraries, subsystemName, commandLine.getValue ("t"));
   Document allClasses (Document::Classes, subsystemName, "Todas las clases");

   collector ();
   compiler (libraries, allClasses);

   Frame mainFrame (subsystemName, libraries.getTitle ());
   mainFrame.set (libraries, allClasses, overview);

   libraries.newLine ();
   libraries.reference ("Resumen", overview.data (), true);
}

void onlinehelp::Application::response ()
   throw (RuntimeException)
{
   Guard guard (a_mutex);

   a_responseCounter ++;
}

void onlinehelp::Application::collector ()
   throw (RuntimeException)
{
   ThreadManager ttmm ("SmartDoc::collector", ThreadManager::Mode::Unlimit, 16);
   Thread* thread (NULL);

   a_responseCounter = 0;

   for (iterator ii = begin (), maxii = end (); ii != maxii; ii ++) {
      thread = ttmm.createThread ();
      thread->start (*library (ii));
   }
}

void onlinehelp::Application::compiler (Document& libraries, Document& allClasses)
   throw (RuntimeException)
{
   Class* theClass;
   Library* library;   
   Library::class_container sortedClasses;
   
   libraries.reference ("Todas las clases", allClasses);
   libraries.newLine ();
   
   for (iterator ii = begin (), maxii = end (); ii < maxii; ii ++) {
      library = Application::library (ii);
      
      if (library->isOk () == false)
         continue;
      
      const std::string& libraryName = library->getName ();            

      Document libraryClasses (Document::Classes, libraryName, libraryName.c_str ());     

      libraries.reference (libraryName, libraryClasses);

      for (Library::class_iterator jj = library->class_begin (), maxjj = library->class_end (); jj < maxjj; jj ++) {
         Class& the_class = Library::the_class (jj);
         sortedClasses.push_back (the_class);
         libraryClasses.reference (the_class);
      }
   }              
   
   sort (sortedClasses.begin (), sortedClasses.end ());   
   for (Library::class_iterator jj = sortedClasses.begin (), maxjj = sortedClasses.end (); jj < maxjj; jj ++) 
      allClasses.reference (Library::the_class (jj));
}


