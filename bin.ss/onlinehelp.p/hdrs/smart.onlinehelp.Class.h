#ifndef smart_onlinehelp_Class_h
#define smart_onlinehelp_Class_h

#include <string>

namespace smart {

namespace onlinehelp {

class Class {
public:
   Class (const char* className, const char* htmlFile) :
      a_className (className), 
      a_htmlFile (htmlFile) 
   {;}
   Class (const std::string& className, const std::string& htmlFile) :
      a_className (className), 
      a_htmlFile (htmlFile) 
   {;}
   Class (const Class& other) :
      a_className (other.a_className), 
      a_htmlFile (other.a_htmlFile) 
   {;}
   
   // Accesores
   const std::string& getClassName () const { return a_className; }
   const std::string& getHTMLFile () const { return a_htmlFile; }
   
   // Operadores
   bool operator == (const Class& right) const { return a_className == right.a_className; }
   
   Class& operator = (const Class& right) {
      if (this != &right) {
         a_className = right.a_className;
         a_htmlFile = right.a_htmlFile;
      }
      
      return *this;
   }
      
private:
   std::string a_className;
   std::string a_htmlFile;
   
   friend bool operator < (const Class &x, const Class &y);
};

inline bool operator < (const Class &x, const Class &y) { return x.a_className < y.a_className; }

}
}


#endif

