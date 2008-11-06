#include <iostream>

#include <nemesis.Exception.h>
#include <nemesis.CommandLine.h>
#include <nemesis.Logger.h>

#include <nemesis.io.sccs.h>

#include <smart.onlinehelp.Application.h>

using namespace std;
using namespace nemesis;
using namespace smart;

int main (int argc, const char** argv)
{
   CommandLine& commandLine (CommandLine::instanciate ());
   onlinehelp::Application app;

   try {
      commandLine.initialize (argv, argc);
      commandLine.verify ();

      Logger::setLevel (Logger::Debug);
      Logger::initialize ("onlinehelp");

      app.start ();
   }
   catch (Exception& ex) {
      cout << ex.asString () << endl << endl;
   }

   return 0;
}
