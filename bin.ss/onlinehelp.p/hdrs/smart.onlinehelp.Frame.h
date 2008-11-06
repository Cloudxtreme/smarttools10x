#ifndef smart_onlinehelp_Frame_h
#define smart_onlinehelp_Frame_h

#include <string>
#include <fstream>

#include <nemesis.RuntimeException.h>

namespace smart {

namespace onlinehelp {


class Document;

class Frame {
public:
   Frame (const std::string& name, const std::string& title);
   ~Frame () { close (); }
   
   void set (const Document& libraries, const Document& classes, const std::string& overview) 
      throw (nemesis::RuntimeException);
      
private:
   std::string a_url;
   std::string a_title;
   std::ofstream a_stream;
   
   void open () throw (nemesis::RuntimeException);
   void close ();
   void reference (const char* text, const char* url) throw (nemesis::RuntimeException);
};

}
}

#endif

