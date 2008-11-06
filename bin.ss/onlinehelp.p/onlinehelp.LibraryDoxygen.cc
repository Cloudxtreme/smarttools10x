#include <iostream>
#include <unistd.h>

#include <nemesis.RuntimeException.h>
#include <nemesis.Tokenizer.h>

#include <nemesis.app.functions.h>

#include <nemesis.io.Directory.h>

#include <smart.onlinehelp.LibraryDoxygen.h>
#include <smart.onlinehelp.Application.h>

using namespace std;
using namespace nemesis;
using namespace smart;

void onlinehelp::LibraryDoxygen::run ()
   throw (RuntimeException)
{
   using namespace nemesis::io;

   onlinehelp::Application& app (static_cast <onlinehelp::Application&> (app::functions::getApp ()));

   Directory directory;
   const char* forbidden [] = {
      "-members", "-source", "main.html", "models.html", "tree.", "index.html", ".png", ".gif", ".jpg", ".dot",
      "__incl", "__graph", NULL
   };
   bool ignore;
   int i;
   Tokenizer tokens;
   string className;
   int pos;

   a_isOk = false;

   using namespace nemesis::io;

   try {
      directory.read (getName (), Directory::Mode::FullPath);

      for (Directory::const_iterator ii = directory.begin (), maxii = directory.end (); ii != maxii; ii ++) {
         const std::string& file (Directory::data (ii));

         for (i = 0, ignore = false; ignore == false && forbidden [i] != NULL; i ++) {
            if (file.find (forbidden [i]) != string::npos)
               ignore = true;
         }

         if (ignore == true)
            continue;

         if (file.find ( "annotated.html") != string::npos) {
            string className (" - Resumen: ");
            className += getName ();
            className += " -";
            a_classes.push_back (Class (className, file));
         }
         else {
            if (tokens (file, "/")  > 0) {
               if (tokens (tokens.last (), ".") > 0) {
                  className = tokens [0];

                  if (className.find ("class") == 0)
                     className.replace (0, 5, "");
                  else if (className.find ("struct") == 0)
                     className.replace (0, 6, "");
                  else
                     continue;

                  do {
                     if ((pos = className.find ("_1_1")) != string::npos)
                        className = className.replace (pos, 4, "::");
                  } while (pos != string::npos);

                  a_classes.push_back (Class (className, file));
               }
            }
         }
      }

      if (a_classes.size () > 0) {
         sort (a_classes.begin (), a_classes.end ());
         a_isOk = true;
      }

      app.response ();
   }
   catch (RuntimeException& ex) {
      cout << ex.asString () << endl;
      app.response ();
      throw;
   }
}

