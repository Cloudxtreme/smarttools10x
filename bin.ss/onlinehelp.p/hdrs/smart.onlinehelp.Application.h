#ifndef _smart_onlinehelp_Application_h
#define _smart_onlinehelp_Application_h

#include <vector>

#include <nemesis.Mutex.h>

#include <nemesis.app.Application.h>

namespace smart {

namespace onlinehelp {

class Library;
class Document;

class Application : public nemesis::app::Application {
public:
   typedef std::vector <Library*> container;
   typedef std::vector <Library*>::iterator iterator;

   Application ();

   void response () throw (nemesis::RuntimeException);
   void initialize () throw (nemesis::RuntimeException);
   void run () throw (nemesis::RuntimeException);
   
private:
   container a_libraries;
   nemesis::Mutex a_mutex;
   int a_responseCounter;
   
   iterator begin () throw () { return a_libraries.begin (); }
   iterator end () throw () { return a_libraries.end (); }
   
   static Library* library (iterator& ii) throw () { return *ii; } 
   
   void collector () throw (nemesis::RuntimeException);
   void compiler (Document& libraries, Document& allClasses) throw (nemesis::RuntimeException);
};

}
}

#endif

