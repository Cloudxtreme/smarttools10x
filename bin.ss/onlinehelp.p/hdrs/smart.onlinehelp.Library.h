#ifndef _smart_onlinehelp_Library_h
#define _smart_onlinehelp_Library_h

#include <vector>

#include <nemesis.Runnable.h>
#include <nemesis.RuntimeException.h>

#include <smart.onlinehelp.Class.h>

namespace smart {

namespace onlinehelp {

using namespace nemesis;

class Library : public nemesis::Runnable {
public:
   typedef std::vector <Class> class_container;
   typedef class_container::iterator class_iterator;

   explicit Library (const std::string& name) : nemesis::Runnable (name) {;}

   // Accesores
   const std::string& getName () const { return getId (); }
   bool isOk () const { return a_isOk; }
   
   // operadores
   bool operator == (const Library& right) const { return getId () == right.getId (); }

   // Metodos
   class_iterator class_begin () throw () { return a_classes.begin (); }
   class_iterator class_end () throw () { return a_classes.end (); }

   static Class& the_class (class_iterator& ii) throw () { return *ii; }
   
protected:
   class_container a_classes;
   bool a_isOk;

   void do_action () throw () {;}
};

}
}

#endif

