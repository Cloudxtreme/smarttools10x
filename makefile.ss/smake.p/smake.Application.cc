/*
   smake - Version 10.0

   Desarrollado por la division de Nuevos conceptos de Plataformas.
   (c) Copyright 2005-2008 Telefonica Investigacion y Desarrollo. S.A. Unipersonal.

   SmartMake, genera los objetos de librerías y ejecutables contenidos en un 
   proyecto determinado. Combina los distintos recursos disponibles para intentar
   optimizar el rendimiento mediante el ILS.

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

#include <signal.h>
#include <unistd.h>
#include <limits.h>
#include <semaphore.h>
#include <stdio.h>
#include <sys/wait.h>

#include <nemesis.Logger.h>
#include <nemesis.functions.h>
#include <nemesis.CommandLine.h>
#include <nemesis.TraceWriter.h>
#include <nemesis.Tokenizer.h>

#include <nemesis.xml.DocumentFile.h>

#include <nemesis.io.TextReader.h>

#include <smart.functions.h>

#include <smart.structure.sccs.h>
#include <smart.structure.Agent.h>

#include <smart.description.functions.h>
#include <smart.description.RuleAgent.h>

#include <smart.smake.Application.h>
#include <smart.smake.Target.h>
#include <smart.smake.Shuffler.h>
#include <smart.smake.sccs.h>

using namespace std;
using namespace nemesis;
using namespace smart;

smart_import_sccs (Maker)

#ifdef __solaris__
int st_child_result = -1;

static void
sigchld_handler (int signum)
{
  waitpid (-1, &st_child_result, WNOHANG);
}
#endif

smake::Application::Application () : 
   app::Application ("SmartTools.Maker", "Generador de ejecutables", smart_use_sccs_version (Maker), __DATE__, __TIME__),
   a_clobber (false),
   a_verbose (false),
   a_directVideo (false),
   a_objectName (NULL)
{
   CommandLine& ccll (CommandLine::instance ());
   
   ccll.add ("d", CommandLine::Argument::Optional, "Documento XML de configuracion");
   ccll.add ("v", CommandLine::Argument::Optional, "Activa el modo verbose", false);
   ccll.add ("vv", CommandLine::Argument::Optional, "La salida del modo verbose salen directamente al terminal", false);
   ccll.add ("t", CommandLine::Argument::Optional, "Nivel de trazas");
   ccll.add ("clobber", CommandLine::Argument::Optional, "Elimina todos los ejecutables y archivos temporales", false);
   ccll.add ("o", CommandLine::Argument::Optional, "Nombre del objeto a crear");
   
   structure::sccs::activate ();
   smake::sccs::activate ();
   
   Variable::disableVerbose ();

   activateGeneralPublicLicense ();
}

void smake::Application::initialize () 
   throw (RuntimeException)
{
   CommandLine& ccll (CommandLine::instanciate ());

   sigignore (SIGPIPE);

   int ret;
   struct sigaction sa;
   struct sigaction osa;

// Para recuperar el código de salida del hijo no hay más-tu-tía que 
// instalar el manejador de señal de salida.
#ifdef __solaris__
   sa.sa_handler = sigchld_handler;
   sigfillset (&sa.sa_mask);
   sa.sa_flags = SA_RESTART;
   ret = sigaction (SIGCHLD, &sa, &osa);
   if (ret < 0)
     {
       printf ("cannot set signal handler, bye!\n");
       exit (-1);
     }
#endif

   if (ccll.exists ("t") == true) 
      Logger::setLevel (Logger::asLevel (ccll.getValue ("t")));

   Logger::initialize (getShortName (), new TraceWriter (".smake.trace", 4096000));   
   
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
   
   a_clobber = ccll.exists ("clobber");

   if ((a_directVideo = ccll.exists ("vv")) == false)
      a_verbose = ccll.exists ("v");
   else 
      a_verbose = true;
   
   if (ccll.exists ("o")) {
      a_objectName = ccll.getValue ("o");
      if (nemesis_strcmp (a_objectName, "clean") == 0) {
         a_objectName = NULL;
         a_clobber = true;
      }
   }

   description::RuleAgent::instance ().config (configuration);      
}

/*
 * (1) Si se direcciona sobre el tty cada vez que el programa invocado haga una salida por pantalla esta se vera
 * reflejada directamente, de otro modo solo se vera cuando el programa escriba un \n.
 */
void smake::Application::run () 
   throw (RuntimeException)
{      
   structure::Agent& structureAgent = structure::Agent::instance ();
   const structure::Section* root = structureAgent.uploadEnvironment ();
   
   const char* targetName = description::functions::getTargetName ().c_str ();
   const std::string strTargetName (targetName);
      
   string expression (description::functions::getTargetPrefix ());
   expression += '{';
   expression += targetName;
   expression += '}';
   expression += description::functions::getTargetExtension ();   
      
   char cwd [PATH_MAX];   
   getcwd (cwd, sizeof (cwd));   
   cout << "Directorio actual: " << cwd << endl << endl;
      
   cbs ();
   
   Target target (strTargetName, expression);
   Shuffler shuffler (target);

   target.initialize (root);
   shuffler.initialize (description::functions::getMakeResource ().c_str ());

   string command;
   string logfile;
   FILE* log = NULL;
   
   try {      
      if (a_verbose == false) {
         logfile = description::functions::getTargetPrefix ();
         logfile += target.getValue ();
         logfile += ".logfile";
         cout << "Fichero de salida " << cwd << "/" << logfile << endl << endl;
         
         if ((log = fopen (logfile.c_str (), "w+")) == NULL)
            throw RuntimeException (logfile, errno, FILE_LOCATION);
      }

      command = "make -rsf ";
      command += description::functions::getMakeName ();
      
      if (a_clobber) 
         command += " clobber";

      if (a_objectName != NULL) {
         command += " ";
         command += a_objectName;
      }
      
      if (a_directVideo == true) {
         string eol ("GMAKE_EOL");
         Shuffler::setEnvironment (eol, "\\n");
      }
            
      char* vtarget;
      bool isok = true;
      
      for (Shuffler::iterator ii = shuffler.begin (), maxii = shuffler.end (); ii != maxii && isok == true; ii ++) {
         if (shuffler.prepareEnvironment (ii) == false)
            continue;
         
         vtarget = getenv (targetName);      
         cout << ((a_clobber) ? "Borrando ": "Generando ") << vtarget << " ... " << endl;
         isok = execute (command, log, vtarget);
         cout << endl;
      }
      
      if (log != NULL)
         fclose (log);      
   }
   catch (RuntimeException& ex) {
      if (log != NULL)
         fclose (log);
      throw;
   }
}

void smart::smake::Application::cbs () 
   throw (RuntimeException)
{
   Variable home ("GMAKE_HOME");
   Variable osname ("GMAKE_OSNAME");
   string path (home.getValue ());
   Tokenizer tokenizer;
   
   path += "/data/";
   path += osname.getValue ();
   path += "/";
   
   string filename (path);
   
   filename += ".keycode";
   const int keycode = getKeyCode (filename);

   LOGDEBUG (
      string msg ("smake::Application::cbs | KeyCode: ");
      msg += filename;
      Logger::debug (msg, FILE_LOCATION);
   );

   Variable arch ("GMAKE_PROCESSORTYPE");
   path += arch.getValue ();
   path += '/';

   filename = path;
   filename += ".checksum";

   LOGDEBUG (
      string msg ("smake::Application::cbs | Checksum: ");
      msg += filename;
      Logger::debug (msg, FILE_LOCATION);
   );
   
   io::TextReader reader;
   
   reader.open (filename);
   
   const char* line;
   int originalCode;
   int currentCode;

   while (line = reader.fetch ()) {
      if (*line == 0)
         continue;

      tokenizer (line, " ");
      
      filename = path;
      filename += tokenizer [0];
      currentCode = smart::functions::keyfile (filename.c_str (), keycode);
      
      sscanf (tokenizer [1].c_str (), "0x%x", &originalCode);
      
      LOGDEBUG (
         string msg ("smake::Application::cbs | File: ");
         msg += filename;
         msg += nemesis::functions::asHexText (" | OriginalCode: ", originalCode);
         msg += nemesis::functions::asHexText (" | Code: ", currentCode);
         Logger::debug (msg, FILE_LOCATION);         
      );
      
      if (currentCode != originalCode) {
         cout << endl << "*** CBS - Change Blocker System ***" << endl;
         cout << "El CBS ayuda a que los archivos que configuran la creacion de ejecutables sean " << endl;
         cout << "modificados solo por personal autorizado." << endl << endl;
         cout << "Solo se podran generar los ejecutables cuando CBS identique que la configuracion" << endl;
         cout << "actual coincide con la realizada en el momento de instalar las SmartTools." << endl << endl;
         cout << "El fichero " << filename << endl;
         cout << "ha sufrido cambios no autorizados." << endl;
         throw RuntimeException ("CBS ha detectado cambios no autorizados en la configuracion", FILE_LOCATION);
      }
   }   
}

int smart::smake::Application::getKeyCode (const std::string& filename) const
   throw (RuntimeException)
{
   io::TextReader keycode;
   int result (0);
   
   keycode.open (filename);
   
   const char* line = keycode.fetch ();
   
   if (line == NULL) {
      string msg (filename);
      msg += " no es valido";
      throw RuntimeException (msg, FILE_LOCATION);
   }
   
   sscanf (line, "0x%x", &result);
   
   LOGDEBUG (
      string msg ("smake::Application::getKeyCode | KeyCode: ");
      msg += nemesis::functions::asHexString (result);
      Logger::debug (msg, FILE_LOCATION);
   );
   
   return result;
}





bool smart::smake::Application::execute (const string& exec, FILE* log, const char* vtarget)
   throw (RuntimeException)
{
   string msg ("smake::Application::execute | Command: ");
   msg += exec;
   msg += " | Target: ";
   msg += vtarget;
   Logger::information (msg, FILE_LOCATION);
   
   if (log != NULL) {
      fprintf (log, "%s\n", msg.c_str ());
      fflush (log);      
   }

   int child_pid;
   
   if ((child_pid = fork ()) < 0)
      throw RuntimeException (exec, errno, FILE_LOCATION);

   /*
    * Lo único que tiene que hacer el "proceso-original" es esperar a que termine
    * el proceso-hijo que acaba de crear, que será el que realmente lance el comando.
    *
    */
   if (child_pid > 0) {
      int child_result = 0;
      
      if (waitpid (child_pid, &child_result, 0) != child_pid) {

#ifdef __solaris__
        // En solaris el código de salida se habrá recuperado en el manejador de señal
        // y al llamar al waitpid obtendremos un código de error ECHILD
        if ((child_result = st_child_result) == -1) {
           RuntimeException ex ("waitpid", errno, FILE_LOCATION);
           ex.trace ();
           child_result = 0;
       }
#else
        RuntimeException ex ("waitpid", errno, FILE_LOCATION);
        ex.trace ();
#endif
      }

      string msg ("smake::Application::execute | Command: ");
      msg += exec;
      msg += " | Target: ";
      msg += vtarget;
      msg += " | Exit Status (";
      msg += nemesis::functions::asHexString (child_result);
      msg += "): ";

      bool result = false;
      
      if (WIFEXITED (child_result)) {
         int status = WEXITSTATUS (child_result);
         result = (status == 0);
         msg += nemesis::functions::asString ("%s (%d)", ((result == true) ? "Ok": "Error"), status);
      }
      else if (WIFSIGNALED (child_result))
         msg += nemesis::functions::asString ("Signal (%d)", WTERMSIG (child_result));
      else if (WIFSTOPPED (child_result))
         msg += nemesis::functions::asString ("Stopped (%d)", WSTOPSIG (child_result));
      
      if (log) {
         fprintf (log, "\n%s\n\n", msg.c_str ());
         fflush (log);
      }

      if (result == false)
         Logger::error (msg, FILE_LOCATION);
      else         
         Logger::information (msg, FILE_LOCATION);

      return result;
   }
   
   /*
    * El proceso-hijo se encarga de establecer el entorno y lanzar el comando.
    * Un shell script no
    */
   char* shell;
   char* argv [4];

   try {
      if ((shell = getenv ("SHELL")) == NULL)
         throw RuntimeException ("Environment variable SHELL is needed to execute shell-scripts", FILE_LOCATION);

      /* Busca al última barra */
      if ((argv [0] = strrchr (shell, '/')) != NULL)
         argv [0] ++;
      else
         argv [0] = shell;

      argv [1] = (char*) "-c";
      argv [2] = (char*) exec.c_str ();
      argv [3] = 0;

      /**
       * Sólo redirecciona la salida estándar. La de error la deja al terminal para que se sigan viendo los 
       * errores detectados durante la compilación.
       */
      if (log) {
         if (dup2 (fileno (log), STDOUT_FILENO) == -1) {
            const int xerrno = errno;
            string msg ("smake::Application::execute (child) | Executable: ");
            msg += shell;
            throw RuntimeException (msg, xerrno, FILE_LOCATION);
         }         
      }

      /*
       * La imagen del proceso-hijo se sustituye por la imagen del shell invocado,
       * pero que copia los fd's asociados a la I/O estándar.
       */
      if (execv (shell, argv) != 0) {
         const int xerrno = errno;
         string msg ("smake::Application::execute (child) | Executable: ");
         msg += shell;
         throw RuntimeException (msg, xerrno, FILE_LOCATION);
      }
   }
   catch (RuntimeException& ex) {
      const char* what = ex.what ();
      if (log) {
         fprintf (log, what);
         fflush (log);
      }
      ex.trace ();
      exit (-1);
   }
   
   // Esta línea no se ejecutará nunca, porque el execv sustituyen la imagen de este proceso.   
   return true;
}

