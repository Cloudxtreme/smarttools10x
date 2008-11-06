#ifndef _smart_onlinehelp_LibraryDoxygen_h
#define _smart_onlinehelp_LibraryDoxygen_h

#include <smart.onlinehelp.Library.h>

namespace smart {

namespace onlinehelp {

class LibraryDoxygen : public Library {
public:
   explicit LibraryDoxygen (const std::string& name) : Library (name) {;}

private:
   void run () throw (nemesis::RuntimeException);
};

}
}

#endif

