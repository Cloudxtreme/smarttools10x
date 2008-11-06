#ifndef _smart_onlinehelp_LibraryKdoc_h
#define _smart_onlinehelp_LibraryKdoc_h

#include <smart.onlinehelp.Library.h>

namespace smart {

namespace onlinehelp {

class LibraryKdoc : public Library {
public:
   explicit LibraryKdoc (const std::string& name) : Library (name) {;}
   
private:   
   void run () throw (nemesis::RuntimeException); 
};

}
}

#endif

