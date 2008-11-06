#include <nemesis.Logger.h>

#include <nemesis.io.TextReader.h>

#include <smart.Variable.h>

#include <smart.snooper.MakefileVariable.h>

using namespace std;
using namespace nemesis;
using namespace smart;

snooper::MakefileVariable::MakefileVariable (QListWidget* parent, const char* name) :
   QListWidgetItem (name, parent),
   a_name (name)
{
}

string snooper::MakefileVariable::asString () const 
   throw ()
{
   string result ("smart::snooper::MakefileVariable { Name: ");
   result += a_name;
   return result += " }";
}
