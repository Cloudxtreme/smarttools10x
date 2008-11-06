#include <nemesis.Logger.h>

#include <nemesis.io.TextReader.h>

#include <smart.Variable.h>

#include <smart.snooper.EnvironVariable.h>

using namespace std;
using namespace nemesis;
using namespace smart;

snooper::EnvironVariable::EnvironVariable (QListWidget* parent, const smart::Variable* variable) :
   QListWidgetItem (variable->getName (), parent),
   a_variable (variable)
{
}

void snooper::EnvironVariable::initialize (const std::string& vpath, const std::string& fullpath) 
   throw (RuntimeException)
{
   a_file = vpath;
   
   io::TextReader reader;
   const char* line;
      
   LOGINFORMATION (
      string msg ("smart::snooper::EnvironVariable::initialize | FullPath: ");
      msg += fullpath;
      msg += " | ";
      msg += asString ();      
      Logger::information (msg, FILE_LOCATION);
   );
   
   reader.open (fullpath);
      
   while ((line = reader.fetch ()) != NULL) {
      a_contain += line;
      a_contain += '\n';
   }
}

const char* snooper::EnvironVariable::getValue () const 
   throw ()
{
   const char* result = "<none>";
   
   try {
      result = a_variable->getValue ().c_str ();
   }
   catch (RuntimeException& ex) {
      ex.trace ();
   }
            
   return result;
}

string snooper::EnvironVariable::asString () const 
   throw ()
{
   string result ("smart::snooper::EnvironVariable { ");
   result += a_variable->asString ();
   result += " | VPath: ";
   result += a_file;
   return result += " }";
}
