#ifndef smart_onlinehelp_Document_h
#define smart_onlinehelp_Document_h

#include <string>
#include <fstream>

#include <nemesis.RuntimeException.h>

namespace smart {

namespace onlinehelp {

class Class;

class Document {
public:
   enum Type { Libraries, Classes };
   
   Document (const Type type, const std::string& name, const char* title = NULL);
   ~Document () { close (); }
   
   const std::string& getTitle () const { return a_title; }
   const std::string& getURL () const { return a_url; }
   
   void reference (const std::string& text, const Document& document) 
      throw (nemesis::RuntimeException)
   {
      reference (text.data (), document.a_url.data ());
   }
   
   void reference (const char* text, const Document& document) 
      throw (nemesis::RuntimeException)
   {
      reference (text, document.a_url.data ());
   }
   
   void reference (const std::string& text, const std::string& url) 
      throw (nemesis::RuntimeException)
   {
      reference (text.data (), url.data ());      
   }
   
   void reference (const Class& theClass) throw (nemesis::RuntimeException);

   void reference (const char* text, const char* url, const bool overview = false) throw (nemesis::RuntimeException);
   
   void newLine () throw (nemesis::RuntimeException);
   
private:
   std::string a_url;
   std::string a_title;
   std::ofstream a_stream;
   Type a_type;
   
   void open () throw (nemesis::RuntimeException);
   void close ();
};

}
}

#endif

